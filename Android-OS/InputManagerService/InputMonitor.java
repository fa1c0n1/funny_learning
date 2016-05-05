/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.server.wm;

import com.android.server.input.InputManagerService;
import com.android.server.input.InputApplicationHandle;
import com.android.server.input.InputWindowHandle;
import com.android.server.wm.WindowManagerService.AllWindowsIterator;

import android.app.ActivityManagerNative;
import android.app.ANRManagerNative;
import android.graphics.Rect;
import android.os.RemoteException;
import android.util.Log;
import android.util.Slog;
import android.view.Display;
import android.view.InputChannel;
import android.view.KeyEvent;
import android.view.WindowManager;

import java.util.ArrayList;
import java.util.Arrays;

/// M: Mobile Manager Service @{
import static com.android.server.wm.WindowManagerService.DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;
import com.android.server.am.ANRManager;
import com.mediatek.common.featureoption.FeatureOption;
import com.mediatek.common.mom.IMobileManagerService;
/// @}

final class InputMonitor implements InputManagerService.WindowManagerCallbacks {
    private final WindowManagerService mService;
    
    // Current window with input focus for keys and other non-touch events.  May be null.
    private WindowState mInputFocus;
    
    // When true, prevents input dispatch from proceeding until set to false again.
    private boolean mInputDispatchFrozen;
    
    // When true, input dispatch proceeds normally.  Otherwise all events are dropped.
    // Initially false, so that input does not get dispatched until boot is finished at
    // which point the ActivityManager will enable dispatching.
    private boolean mInputDispatchEnabled;

    // When true, need to call updateInputWindowsLw().
    private boolean mUpdateInputWindowsNeeded = true;

    // Array of window handles to provide to the input dispatcher.
    private InputWindowHandle[] mInputWindowHandles;
    private int mInputWindowHandleCount;

    // Set to true when the first input device configuration change notification
    // is received to indicate that the input devices are ready.
    private final Object mInputDevicesReadyMonitor = new Object();
    private boolean mInputDevicesReady;

    /// M: Mobile Manager Service
    private static final long MILLI_TO_NANO = 1000*1000;

    public InputMonitor(WindowManagerService service) {
        mService = service;
    }
    
    /* Notifies the window manager about a broken input channel.
     * 
     * Called by the InputManager.
     */
    public void notifyInputChannelBroken(InputWindowHandle inputWindowHandle) {
        if (inputWindowHandle == null) {
            return;
        }

        synchronized (mService.mWindowMap) {
            WindowState windowState = (WindowState) inputWindowHandle.windowState;
            if (windowState != null) {
                Slog.i(WindowManagerService.TAG, "WINDOW DIED " + windowState);
                mService.removeWindowLocked(windowState.mSession, windowState);
            }
        }
    }

    /// M: Enhance keydispatching predump @{
    public void notifyPredump (InputApplicationHandle inputApplicationHandle, InputWindowHandle inputWindowHandle, int pid, int message)
    {
        try 
        {
            WindowState windowState = null;
            AppWindowToken appWindowToken = null;
            if (inputWindowHandle != null)
            {
                windowState = (WindowState) inputWindowHandle.windowState;
                if (windowState != null)
                {
                    appWindowToken = windowState.mAppToken;
                }
            }
            // Add windowState!= null condition for notifyANR new case
            if(appWindowToken != null || inputApplicationHandle!= null || windowState != null)
            {
                //mService.mActivityManager.notifyLightWeightANR(pid, "KeyDispatchingTimeout predump", message);
                ANRManagerNative.getDefault().notifyLightWeightANR(pid, "KeyDispatchingTimeout predump", message);
            }
            else
            {
                Slog.i(WindowManagerService.TAG, "Touch event for WNR, it isn't necessary to predump");
            }
        }
        catch (Exception e)
        {
            Slog.w(WindowManagerService.TAG, "Error notifyPredump ", e);
        }
    }
    /// @}

    
    /* Notifies the window manager about an application that is not responding.
     * Returns a new timeout to continue waiting in nanoseconds, or 0 to abort dispatch.
     * 
     * Called by the InputManager.
     */
    public long notifyANR(InputApplicationHandle inputApplicationHandle,
            InputWindowHandle inputWindowHandle) {
        AppWindowToken appWindowToken = null;
        WindowState windowState = null;
        boolean aboveSystem = false;
        /// M: 2012-07-06 ALPS00314133 WNR Debugging Mechanism
        boolean bIsWNR = false;
        synchronized (mService.mWindowMap) {
            if (inputWindowHandle != null) {
                windowState = (WindowState) inputWindowHandle.windowState;
                if (windowState != null) {
                    Slog.i(WindowManagerService.TAG, "Input event dispatching timed out sending to "
                            + windowState.mAttrs.getTitle());
                    appWindowToken = windowState.mAppToken;
                    /// M: 2012-07-06 ALPS00314133 WNR Debugging Mechanism @{
                    if(appWindowToken == null)
                    {
                        bIsWNR = true;
                    }
                    /// @}
                    
                    /// M: Dump input dispatching status in ViewRoot.@{
                    try {
                        windowState.mClient.dumpInputDispatchingStatus();
                    } catch (RemoteException e) {
                        Slog.w(WindowManagerService.TAG, "Error dump input dispatching status.", e);
                    }
                    /// @}
                }
            }
            if (appWindowToken == null && inputApplicationHandle != null) {
                appWindowToken = (AppWindowToken)inputApplicationHandle.appWindowToken;
            }

            if (windowState != null) {
                Slog.i(WindowManagerService.TAG, "Input event dispatching timed out "
                        + "sending to " + windowState.mAttrs.getTitle());
                // Figure out whether this window is layered above system windows.
                // We need to do this here to help the activity manager know how to
                // layer its ANR dialog.
                int systemAlertLayer = mService.mPolicy.windowTypeToLayerLw(
                        WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
                aboveSystem = windowState.mBaseLayer > systemAlertLayer;
            } else if (appWindowToken != null) {
                Slog.i(WindowManagerService.TAG, "Input event dispatching timed out "
                        + "sending to application " + appWindowToken.stringName);
            } else {
                Slog.i(WindowManagerService.TAG, "Input event dispatching timed out.");
            }

            mService.saveANRStateLocked(appWindowToken, windowState);
        }

        if (appWindowToken != null && appWindowToken.appToken != null) {
            try {
                // Notify the activity manager about the timeout and let it decide whether
                // to abort dispatching or keep waiting.
                //boolean abort = appWindowToken.appToken.keyDispatchingTimedOut();

                /// M: ANR error handling for MoMS @{
                if(FeatureOption.MTK_MOBILE_MANAGEMENT) {
                    long extendTime = ANRManager.extendANRTime(appWindowToken.inputDispatchingTimeoutNanos/MILLI_TO_NANO);
                    if (extendTime > 0) {
                        Slog.w(WindowManagerService.TAG, "Skip INPUT_DISPATCH_TIMEOUT ANR due to user confirm blocking");
                        return extendTime*MILLI_TO_NANO;
                    }
                }
                /// @}

                boolean abort = true;
                /// M: 2012-07-06 ALPS00314133 WNR Debugging Mechanism @{
                if(bIsWNR) 
                {
                    Slog.i(WindowManagerService.TAG, "notifyWNR mPid="+windowState.mSession.mPid);
                    //abort = mService.mActivityManager.notifyWNR(windowState.mSession.mPid, "KeyDispatchingTimeout--WNR");
                    abort = ANRManagerNative.getDefault().notifyWNR(windowState.mSession.mPid, "KeyDispatchingTimeout--WNR");
                }
                else
                {
                    abort = appWindowToken.appToken.keyDispatchingTimedOut();
                }
                /// @}
                if (! abort) {
                    // The activity manager declined to abort dispatching.
                    // Wait a bit longer and timeout again later.
                    return appWindowToken.inputDispatchingTimeoutNanos;
                }
            } catch (RemoteException ex) {
            }
        } else if (windowState != null) {
            try {
                // Notify the activity manager about the timeout and let it decide whether
                // to abort dispatching or keep waiting.
                long timeout = ActivityManagerNative.getDefault().inputDispatchingTimedOut(
                        windowState.mSession.mPid, aboveSystem);
                if (timeout >= 0) {
                    // The activity manager declined to abort dispatching.
                    // Wait a bit longer and timeout again later.
                    return timeout;
                }

                /// M: ANR error handling for MoMS @{
                if(FeatureOption.MTK_MOBILE_MANAGEMENT) {
                    long extendTime = ANRManager.extendANRTime(DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS/MILLI_TO_NANO);
                    if (extendTime > 0) {
                        Slog.w(WindowManagerService.TAG, "Skip INPUT_DISPATCH_TIMEOUT ANR due to user confirm blocking");
                        return extendTime*MILLI_TO_NANO;
                    }
                }
                /// @}
            } catch (RemoteException ex) {
            }
        }
        /// M: Enhance keydispatching predump @{
        else {
            Slog.i(WindowManagerService.TAG, "both windowState & appWindowToken are null");
        }
        /// @}
        return 0; // abort dispatching
    }

    private void addInputWindowHandleLw(final InputWindowHandle windowHandle) {
        if (mInputWindowHandles == null) {
            mInputWindowHandles = new InputWindowHandle[16];
        }
        if (mInputWindowHandleCount >= mInputWindowHandles.length) {
            mInputWindowHandles = Arrays.copyOf(mInputWindowHandles,
                    mInputWindowHandleCount * 2);
        }
        mInputWindowHandles[mInputWindowHandleCount++] = windowHandle;
    }

    private void addInputWindowHandleLw(final InputWindowHandle inputWindowHandle,
            final WindowState child, final int flags, final int type,
            final boolean isVisible, final boolean hasFocus, final boolean hasWallpaper) {
        // Add a window to our list of input windows.
        inputWindowHandle.name = child.toString();
        inputWindowHandle.layoutParamsFlags = flags;
        inputWindowHandle.layoutParamsType = type;
        inputWindowHandle.dispatchingTimeoutNanos = child.getInputDispatchingTimeoutNanos();
        inputWindowHandle.visible = isVisible;
        inputWindowHandle.canReceiveKeys = child.canReceiveKeys();
        inputWindowHandle.hasFocus = hasFocus;
        inputWindowHandle.hasWallpaper = hasWallpaper;
        inputWindowHandle.paused = child.mAppToken != null ? child.mAppToken.paused : false;
        inputWindowHandle.layer = child.mLayer;
        inputWindowHandle.ownerPid = child.mSession.mPid;
        inputWindowHandle.ownerUid = child.mSession.mUid;
        inputWindowHandle.inputFeatures = child.mAttrs.inputFeatures;

        final Rect frame = child.mFrame;
        inputWindowHandle.frameLeft = frame.left;
        inputWindowHandle.frameTop = frame.top;
        inputWindowHandle.frameRight = frame.right;
        inputWindowHandle.frameBottom = frame.bottom;

        if (child.mGlobalScale != 1) {
            // If we are scaling the window, input coordinates need
            // to be inversely scaled to map from what is on screen
            // to what is actually being touched in the UI.
            inputWindowHandle.scaleFactor = 1.0f/child.mGlobalScale;
        } else {
            inputWindowHandle.scaleFactor = 1;
        }

        child.getTouchableRegion(inputWindowHandle.touchableRegion);

        addInputWindowHandleLw(inputWindowHandle);
    }

    private void clearInputWindowHandlesLw() {
        while (mInputWindowHandleCount != 0) {
            mInputWindowHandles[--mInputWindowHandleCount] = null;
        }
    }

    public void setUpdateInputWindowsNeededLw() {
        mUpdateInputWindowsNeeded = true;
    }

    /* Updates the cached window information provided to the input dispatcher. */
    public void updateInputWindowsLw(boolean force) {
        if (!force && !mUpdateInputWindowsNeeded) {
            return;
        }
        mUpdateInputWindowsNeeded = false;

        if (false) Slog.d(WindowManagerService.TAG, ">>>>>> ENTERED updateInputWindowsLw");

        // Populate the input window list with information about all of the windows that
        // could potentially receive input.
        // As an optimization, we could try to prune the list of windows but this turns
        // out to be difficult because only the native code knows for sure which window
        // currently has touch focus.
        final WindowStateAnimator universeBackground = mService.mAnimator.mUniverseBackground;
        final int aboveUniverseLayer = mService.mAnimator.mAboveUniverseLayer;
        boolean addedUniverse = false;

        // If there's a drag in flight, provide a pseudowindow to catch drag input
        final boolean inDrag = (mService.mDragState != null);
        if (inDrag) {
            if (WindowManagerService.DEBUG_DRAG) {
                Log.d(WindowManagerService.TAG, "Inserting drag window");
            }
            final InputWindowHandle dragWindowHandle = mService.mDragState.mDragWindowHandle;
            if (dragWindowHandle != null) {
                addInputWindowHandleLw(dragWindowHandle);
            } else {
                Slog.w(WindowManagerService.TAG, "Drag is in progress but there is no "
                        + "drag window handle.");
            }
        }

        final int NFW = mService.mFakeWindows.size();
        for (int i = 0; i < NFW; i++) {
            addInputWindowHandleLw(mService.mFakeWindows.get(i).mWindowHandle);
        }

        // Add all windows on the default display.
        final AllWindowsIterator iterator = mService.new AllWindowsIterator(
                WindowManagerService.REVERSE_ITERATOR);
        while (iterator.hasNext()) {
            final WindowState child = iterator.next();
            final InputChannel inputChannel = child.mInputChannel;
            final InputWindowHandle inputWindowHandle = child.mInputWindowHandle;
            if (inputChannel == null || inputWindowHandle == null || child.mRemoved) {
                // Skip this window because it cannot possibly receive input.
                continue;
            }
            
            final int flags = child.mAttrs.flags;
            final int type = child.mAttrs.type;
            
            final boolean hasFocus = (child == mInputFocus);
            final boolean isVisible = child.isVisibleLw();
            final boolean hasWallpaper = (child == mService.mWallpaperTarget)
                    && (type != WindowManager.LayoutParams.TYPE_KEYGUARD);
            final boolean onDefaultDisplay = (child.getDisplayId() == Display.DEFAULT_DISPLAY);

            // If there's a drag in progress and 'child' is a potential drop target,
            // make sure it's been told about the drag
            if (inDrag && isVisible && onDefaultDisplay) {
                mService.mDragState.sendDragStartedIfNeededLw(child);
            }

            if (universeBackground != null && !addedUniverse
                    && child.mBaseLayer < aboveUniverseLayer && onDefaultDisplay) {
                final WindowState u = universeBackground.mWin;
                if (u.mInputChannel != null && u.mInputWindowHandle != null) {
                    addInputWindowHandleLw(u.mInputWindowHandle, u, u.mAttrs.flags,
                            u.mAttrs.type, true, u == mInputFocus, false);
                }
                addedUniverse = true;
            }

            if (child.mWinAnimator != universeBackground) {
                addInputWindowHandleLw(inputWindowHandle, child, flags, type,
                        isVisible, hasFocus, hasWallpaper);
            }
        }

        // Send windows to native code.
        mService.mInputManager.setInputWindows(mInputWindowHandles);

        // Clear the list in preparation for the next round.
        clearInputWindowHandlesLw();

        if (false) Slog.d(WindowManagerService.TAG, "<<<<<<< EXITED updateInputWindowsLw");
    }

    /* Notifies that the input device configuration has changed. */
    public void notifyConfigurationChanged() {
        mService.sendNewConfiguration();

        synchronized (mInputDevicesReadyMonitor) {
            if (!mInputDevicesReady) {
                mInputDevicesReady = true;
                mInputDevicesReadyMonitor.notifyAll();
            }
        }
    }

    /* Waits until the built-in input devices have been configured. */
    public boolean waitForInputDevicesReady(long timeoutMillis) {
        synchronized (mInputDevicesReadyMonitor) {
            if (!mInputDevicesReady) {
                try {
                    mInputDevicesReadyMonitor.wait(timeoutMillis);
                } catch (InterruptedException ex) {
                }
            }
            return mInputDevicesReady;
        }
    }

    /* Notifies that the lid switch changed state. */
    public void notifyLidSwitchChanged(long whenNanos, boolean lidOpen) {
        mService.mPolicy.notifyLidSwitchChanged(whenNanos, lidOpen);
    }
    
    /* Provides an opportunity for the window manager policy to intercept early key
     * processing as soon as the key has been read from the device. */
    public int interceptKeyBeforeQueueing(
            KeyEvent event, int policyFlags, boolean isScreenOn) {
        return mService.mPolicy.interceptKeyBeforeQueueing(event, policyFlags, isScreenOn);
    }

    /* Provides an opportunity for the window manager policy to intercept early
     * motion event processing when the screen is off since these events are normally
     * dropped. */
    public int interceptMotionBeforeQueueingWhenScreenOff(int policyFlags) {
        return mService.mPolicy.interceptMotionBeforeQueueingWhenScreenOff(policyFlags);
    }

    /* Provides an opportunity for the window manager policy to process a key before
     * ordinary dispatch. */
    public long interceptKeyBeforeDispatching(
            InputWindowHandle focus, KeyEvent event, int policyFlags) {
        WindowState windowState = focus != null ? (WindowState) focus.windowState : null;
        return mService.mPolicy.interceptKeyBeforeDispatching(windowState, event, policyFlags);
    }
    
    /* Provides an opportunity for the window manager policy to process a key that
     * the application did not handle. */
    public KeyEvent dispatchUnhandledKey(
            InputWindowHandle focus, KeyEvent event, int policyFlags) {
        WindowState windowState = focus != null ? (WindowState) focus.windowState : null;
        return mService.mPolicy.dispatchUnhandledKey(windowState, event, policyFlags);
    }

    /* Callback to get pointer layer. */
    public int getPointerLayer() {
        return mService.mPolicy.windowTypeToLayerLw(WindowManager.LayoutParams.TYPE_POINTER)
                * WindowManagerService.TYPE_LAYER_MULTIPLIER
                + WindowManagerService.TYPE_LAYER_OFFSET;
    }

    /* Called when the current input focus changes.
     * Layer assignment is assumed to be complete by the time this is called.
     */
    public void setInputFocusLw(WindowState newWindow, boolean updateInputWindows) {
        if (WindowManagerService.DEBUG_INPUT) {
            Slog.d(WindowManagerService.TAG, "Input focus has changed to " + newWindow);
        }

        if (newWindow != mInputFocus) {
            if (newWindow != null && newWindow.canReceiveKeys()) {
                // Displaying a window implicitly causes dispatching to be unpaused.
                // This is to protect against bugs if someone pauses dispatching but
                // forgets to resume.
                newWindow.mToken.paused = false;
            }

            mInputFocus = newWindow;
            setUpdateInputWindowsNeededLw();

            if (updateInputWindows) {
                updateInputWindowsLw(false /*force*/);
            }
        }
    }
    
    public void setFocusedAppLw(AppWindowToken newApp) {
        // Focused app has changed.
        if (newApp == null) {
            mService.mInputManager.setFocusedApplication(null);
        } else {
            final InputApplicationHandle handle = newApp.mInputApplicationHandle;
            handle.name = newApp.toString();
            handle.dispatchingTimeoutNanos = newApp.inputDispatchingTimeoutNanos;
             /// M: 20120712 ALPS00317478 KeyDispatchingTimeout predump Mechanism @{
            try 
            {
                handle.pid = newApp.appToken.getFocusAppPid();
            } 
            catch (Exception ex) 
            {
                Slog.e(WindowManagerService.TAG, "GetFocusAppPid fail");
            }
			/// @}
            mService.mInputManager.setFocusedApplication(handle);
        }
    }
    
    public void pauseDispatchingLw(WindowToken window) {
        if (! window.paused) {
            if (WindowManagerService.DEBUG_INPUT) {
                Slog.v(WindowManagerService.TAG, "Pausing WindowToken " + window);
            }
            
            window.paused = true;
            updateInputWindowsLw(true /*force*/);
        }
    }
    
    public void resumeDispatchingLw(WindowToken window) {
        if (window.paused) {
            if (WindowManagerService.DEBUG_INPUT) {
                Slog.v(WindowManagerService.TAG, "Resuming WindowToken " + window);
            }
            
            window.paused = false;
            updateInputWindowsLw(true /*force*/);
        }
    }
    
    public void freezeInputDispatchingLw() {
        if (! mInputDispatchFrozen) {
            if (WindowManagerService.DEBUG_INPUT) {
                Slog.v(WindowManagerService.TAG, "Freezing input dispatching");
            }
            
            mInputDispatchFrozen = true;
            updateInputDispatchModeLw();
        }
    }
    
    public void thawInputDispatchingLw() {
        if (mInputDispatchFrozen) {
            if (WindowManagerService.DEBUG_INPUT) {
                Slog.v(WindowManagerService.TAG, "Thawing input dispatching");
            }
            
            mInputDispatchFrozen = false;
            updateInputDispatchModeLw();
        }
    }
    
    public void setEventDispatchingLw(boolean enabled) {
        if (mInputDispatchEnabled != enabled) {
            if (WindowManagerService.DEBUG_INPUT) {
                Slog.v(WindowManagerService.TAG, "Setting event dispatching to " + enabled);
            }
            
            mInputDispatchEnabled = enabled;
            updateInputDispatchModeLw();
        }
    }
    
    private void updateInputDispatchModeLw() {
        mService.mInputManager.setInputDispatchMode(mInputDispatchEnabled, mInputDispatchFrozen);
    }
}
