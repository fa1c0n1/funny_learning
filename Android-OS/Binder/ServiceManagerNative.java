/*
 * Copyright (C) 2006 The Android Open Source Project
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

package android.os;

import java.util.ArrayList;


/**
 * Native implementation of the service manager.  Most clients will only
 * care about getDefault() and possibly asInterface().
 * @hide
 */
public abstract class ServiceManagerNative extends Binder implements IServiceManager
{
    /**
     * Cast a Binder object into a service manager interface, generating
     * a proxy if needed.
     */
    /*
     * 这里的asInterface方法与Native层Binder中的interface_cast宏的作用类似，
     *   Java层：以BinderProxy对象为参数构造一个与业务相关的Proxy对象--ServiceManagerProxy.
     *   Native层: 以BpBinder对象为参数构造一个与业务相关的Proxy对象--BpServiceManager.
     */
    static public IServiceManager asInterface(IBinder obj)
    {
        if (obj == null) {
            return null;
        }
        IServiceManager in =
            (IServiceManager)obj.queryLocalInterface(descriptor);
        if (in != null) {
            return in;
        }
        
        /*
         * ServiceManagerProxy对象的各个业务函数会将相应的请求打包交给
         * BinderProxy对象，该对象再调用native方法，最终由Native层的BpBinder
         * 对象发送给Binder驱动以完成一次通信。
         *
         * PS: 实际上，通过之前对Native层的Binder分析可知，BpBinder对象也不会直接
         *     和Binder驱动交互，真正和Binder驱动交互的是IPCThreadState
         */
        return new ServiceManagerProxy(obj);
    }
    
    public ServiceManagerNative()
    {
        attachInterface(this, descriptor);
    }
    
    public boolean onTransact(int code, Parcel data, Parcel reply, int flags)
    {
        try {
            switch (code) {
            case IServiceManager.GET_SERVICE_TRANSACTION: {
                data.enforceInterface(IServiceManager.descriptor);
                String name = data.readString();
                IBinder service = getService(name);
                reply.writeStrongBinder(service);
                return true;
            }
    
            case IServiceManager.CHECK_SERVICE_TRANSACTION: {
                data.enforceInterface(IServiceManager.descriptor);
                String name = data.readString();
                IBinder service = checkService(name);
                reply.writeStrongBinder(service);
                return true;
            }
    
            case IServiceManager.ADD_SERVICE_TRANSACTION: {
                data.enforceInterface(IServiceManager.descriptor);
                String name = data.readString();
                IBinder service = data.readStrongBinder();
                boolean allowIsolated = data.readInt() != 0;
                addService(name, service, allowIsolated);
                return true;
            }
    
            case IServiceManager.LIST_SERVICES_TRANSACTION: {
                data.enforceInterface(IServiceManager.descriptor);
                String[] list = listServices();
                reply.writeStringArray(list);
                return true;
            }
            
            case IServiceManager.SET_PERMISSION_CONTROLLER_TRANSACTION: {
                data.enforceInterface(IServiceManager.descriptor);
                IPermissionController controller
                        = IPermissionController.Stub.asInterface(
                                data.readStrongBinder());
                setPermissionController(controller);
                return true;
            }
            }
        } catch (RemoteException e) {
        }
        
        return false;
    }

    public IBinder asBinder()
    {
        return this;
    }
}

class ServiceManagerProxy implements IServiceManager {
    public ServiceManagerProxy(IBinder remote) {
        mRemote = remote;
    }
    
    public IBinder asBinder() {
        return mRemote;
    }
    
    public IBinder getService(String name) throws RemoteException {
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        data.writeInterfaceToken(IServiceManager.descriptor);
        data.writeString(name);
        mRemote.transact(GET_SERVICE_TRANSACTION, data, reply, 0);
        IBinder binder = reply.readStrongBinder();
        reply.recycle();
        data.recycle();
        return binder;
    }

    public IBinder checkService(String name) throws RemoteException {
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        data.writeInterfaceToken(IServiceManager.descriptor);
        data.writeString(name);
        mRemote.transact(CHECK_SERVICE_TRANSACTION, data, reply, 0);
        IBinder binder = reply.readStrongBinder();
        reply.recycle();
        data.recycle();
        return binder;
    }

    public void addService(String name, IBinder service, boolean allowIsolated)
            throws RemoteException {
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        data.writeInterfaceToken(IServiceManager.descriptor);
        data.writeString(name);

        //注意下面这个writeStrongBinder函数,
        data.writeStrongBinder(service);
        data.writeInt(allowIsolated ? 1 : 0);

        //mRemote实际上就是BinderProxy对象
        //调用它的native方法transact，将封装好的请求数据发送出去
        mRemote.transact(ADD_SERVICE_TRANSACTION, data, reply, 0);
        reply.recycle();
        data.recycle();
    }
    
    public String[] listServices() throws RemoteException {
        ArrayList<String> services = new ArrayList<String>();
        int n = 0;
        while (true) {
            Parcel data = Parcel.obtain();
            Parcel reply = Parcel.obtain();
            data.writeInterfaceToken(IServiceManager.descriptor);
            data.writeInt(n);
            n++;
            try {
                boolean res = mRemote.transact(LIST_SERVICES_TRANSACTION, data, reply, 0);
                if (!res) {
                    break;
                }
            } catch (RuntimeException e) {
                // The result code that is returned by the C++ code can
                // cause the call to throw an exception back instead of
                // returning a nice result...  so eat it here and go on.
                break;
            }
            services.add(reply.readString());
            reply.recycle();
            data.recycle();
        }
        String[] array = new String[services.size()];
        services.toArray(array);
        return array;
    }

    public void setPermissionController(IPermissionController controller)
            throws RemoteException {
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        data.writeInterfaceToken(IServiceManager.descriptor);
        data.writeStrongBinder(controller.asBinder());
        mRemote.transact(SET_PERMISSION_CONTROLLER_TRANSACTION, data, reply, 0);
        reply.recycle();
        data.recycle();
    }

    private IBinder mRemote;
}
