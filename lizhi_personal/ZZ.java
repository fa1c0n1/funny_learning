1. 银商U架构通讯模块的实现。
2. 银商U架构应用升级接口的实现。
3. 银商U架构各个模块的一些bug的修复。
4. 银商U架构测试客户端UMSClient中测试案例的添加、修改等。
5. 协助王义东关于扫描头模块测试应用的bug。
6. 银商招标测试。


详情：
  1. 银商U架构通讯模块的实现：
     实现以太网, wifi和gprs/3G网络的参数设置、打开和关闭功能。
  2. 银商U架构应用升级接口的实现：
     实现apk的静默安装并在安装完成后自动启动新版本的应用。
  3. 银商U架构各个模块的一些bug的修复，如：
     //----通讯模块----
     (1) 频繁切换网络会重复创建多个相同的apn
         解决：增加对已存在数据库中的apn名称进行过滤
     (2) 首次进入客户端，调用open打开wifi出现服务端崩溃
         解决：增加对空参数的过滤处理
     (3) 移动网络apn名称设置为apn后，调用打开移动网络连接指定apn失败
         解决：修改将apn插入数据库中的sql语句
     (4) 调用打开以太网，回调方法onOpenResult中发生NullPointerException导致服务端崩溃
         解决：服务端没有对NullPointerException进行捕获
     (5) 将apn名称gprs_apn参数设置为 @#$%”*-/()!”`:;,? 后调用open(2)打开gprs，服务端崩溃
         解决：修改将apn插入数据库中的sql语句，实现对特殊符号的过滤
     (6) 不插入sim卡，设置apn后调用open打开移动网络，服务端崩溃
         解决：增加了对sim卡的状态的过滤处理
     (7) 多次调用scanWifi，发现一直回显wifi热点的列表根本停不下来.
         解决：对自定义的wifi广播接收者的对象数进行控制，为null时才创建新对象.

     //----服务引擎模块----
     (1) 系统时间设置接口setSystemClock()的实现，传空参数时导致服务端崩溃
          解决：增加对空参数的过滤处理。
     (2) 应用升级接口installApp()的实现，传空参数时导致服务端崩溃
          解决：增加对空参数的过滤处理。
     (3) 接口installApp在data/data/目录及外置SD卡目录下升级不成功。
          解决：由于UMSServer需要system权限，而system用户是无法对外置SD卡进行读写操作的;
                另外，放置在/data/data目录下，由于银商不允许在UMSServer代码里执行chmod指令,
                就是说默认银商会放置在一个system用户可以读写的目录下。

     //----扫描头模块----
     (1) 连续调用startScan两次，第一次和第二次启动扫描头均不正常，会卡顿一下然后退出
         解决： 在代码中做了处理让其不能连续启动两次，只能是先结束本次扫描才能再次启动扫描。

     //----串口模块----
     (1) 串口打开后，循环发送1-8k数据，每次发送完就接收，但是发送到143长度数据时，send 函数卡住了。（统共发送了10153长度数据，即使在每次发送前调用clrBuffer结果也一样）
          解决：发现是测试代码的bug.

     //----Modem模块----
     (1) Connect在被叫无应答，被叫忙和被叫不接电话线等异常情况下，retCode无返回值.
          解决：发现是测试代码的bug

     //----打印机模块----
     (1) 打印机过热，有时OnPrintListener的onPrintResult里的retCode为8，应为-1008
          解决：对错误码进行了转换。
     (2) appendPrnStr返回0，OnPrintListener的onPrintResult里的retCode为-1，getStatus返回0，应均为-1011.
          解决：增加了对打印数据点行高度的判断，当高度大于3000时就返回打印数据过长的错误码。


     //----IC读卡器模块----
     


     //----CPU卡模块----
     bug描述：
       在银商的应用AnyPay上进行CPU卡插卡测试，会随机出现如下错误：
       	  1) APDU通讯错误
   		  2) 卡片初始化失败

   	   正常应该是会出现如下提示：
   	       开始请求插卡
		   用户插卡，获取到CPU卡
           APDU指令 (一串数字(略) 结果：9000

       分析:
           通过错误情况的打印日志观察可知，出现以上错误时，mIccInitCfg，有时为0x00,有时为0x02, 有时为0x03.      
           正常情况下，由于插的槽位为IC卡槽，所以传入的slotNo为0，那么通过getInstance()初始化后，mIccInitCfg的值应为0x00才对.
           那么，问题是为什么还时而出现0x02，0x03的情况。可以想到银商在插卡处理的时候，启动了3个线程来分别对IC卡(支持槽号为0)，
           SAM卡(支持槽号为2,3).进行实例初始化. 因为当时在写getInstance()这个方法的时候没有考虑到银商会这样做，
           getInstance()方法的代码如下，假设getInstance(0)后，iccInstance已经不为null，那这时如果再getInstance(2)，
           getInstance(3)，iccChNum不同了，则会直接将iccInstance的mIccInitCfg进行修改.所以会导致IC卡槽上的CPU卡读卡出错.
           既然银商这么做， 为了避免出现这样的错误，这里改进的方法是，根据传进来的槽号，分别创建不同的instance对象，
           改进后的代码如下:

           //原来
           public synchronized static IccCardReaderService getInstance(int slotNo) {
               IccCardReaderService instance = null;
               if (slotNo == IccReaderSlot.RFSlOT) {
                   if (piccInstance == null) {
                       piccInstance = new IccCardReaderService();
                       try {
                           PiccManager.getInstance().piccClose();
                           PiccManager.getInstance().piccOpen();
                       } catch (PiccException e) {
                           e.printStackTrace();
                       }
                       piccInstance.mSlotNo = slotNo;
                   }
                   instance = piccInstance;
               }else if (slotNo == IccReader.ICSlOT1
                       || slotNo == IccReaderSlot.PSAMSlOT1
                       || slotNo == IccReaderSlot.PSAMSlOT2){
                   if (iccInstance == null) {
                       iccInstance = new IccCardReaderService();
                   }
                   instance =  iccInstance;
                   byte iccChNum = PAX_ICC_CHANNEL_ICC1;
                   iccInstance.mSlotNo = slotNo;
                   iccInstance.mIccInitCfg &= ~0x7;
                   iccInstance.mIccInitCfg |= (iccChNum << 0);
               }else {
                   PaxDebug.LOG("IccCardReaderService:getInstance", "slotNo " + slotNo + " is not support yet.");
               }

               return instance;
           }


           //改进后
           public synchronized static IccCardReaderService getInstance(int slotNo) {
               IccCardReaderService instance = null;
               if (slotNo == IccReaderSlot.RFSlOT) {
                   if (piccInstance == null) {
                       piccInstance = new IccCardReaderService();
                       try {
                           PiccManager.getInstance().piccClose();
                           PiccManager.getInstance().piccOpen();
                       } catch (PiccException e) {
                           e.printStackTrace();
                       }
                       piccInstance.mSlotNo = slotNo;
                   }
                   instance = piccInstance;
               }else if (slotNo == IccReaderSlot.ICSlOT1) {//目前ic只支持ICSlOT1 PSAMSlOT1 和 PSAMSlOT2
                   if (iccInstance == null) {
                       iccInstance = new IccCardReaderService();
                   }
                   instance =  iccInstance;
                   byte iccChNum = PAX_ICC_CHANNEL_ICC1;
                   iccInstance.mSlotNo = slotNo;
                   iccInstance.mIccInitCfg &= ~0x7;
                   iccInstance.mIccInitCfg |= (iccChNum << 0);
               }else if (slotNo == IccReaderSlot.PSAMSlOT1) {
                   if (psam1Instance == null) {
                       psam1Instance = new IccCardReaderService();
                   }
                   instance = psam1Instance;
                   byte iccChNum = PAX_ICC_CHANNEL_PSAM1;
                   psam1Instance.mSlotNo = slotNo;
                   psam1Instance.mIccInitCfg &= ~0x7;
                   psam1Instance.mIccInitCfg |= (iccChNum << 0);
               }else if (slotNo == IccReaderSlot.PSAMSlOT2) {
                   if (psam2Instance == null) {
                       psam2Instance = new IccCardReaderService();
                   }
                   instance = psam2Instance;
                   byte iccChNum = PAX_ICC_CHANNEL_PSAM2;
                   psam2Instance.mSlotNo = slotNo;
                   psam2Instance.mIccInitCfg &= ~0x7;
                   psam2Instance.mIccInitCfg |= (iccChNum << 0);
               }else {
                   PaxDebug.LOG("IccCardReaderService:getInstance", "slotNo " + slotNo + " is not support yet.");
               }

               return instance;
           }



   4. 银商U架构测试客户端UMSClient中测试案例的添加、修改等。
      略。

   5. 协助王义东关于扫描头模块测试应用的bug。
      问题描述如下：
        退出应用后，如果在机器的任务管理器里面把应用关闭，再次启动能够正常运行。如果直接再次启动，则不能正常运行。不正常表现为扫描结束后我的activity不知道跑到那里了，表现为
        里面的Handle接收的信息没有显示在Activity中。

      原因及解决：
        原因就是handler和activity被ScanTest引用了,从而阻止了GC去回收handler对象和activity对象。
        只要退出activity的时候把ScanTest里面的mContext和mHandle置为null就OK了。


   6. 银商招标测试。
      应用的升级出现问题，升级后就直接返回桌面了，并没有将新版本的应用启动。
      错误日志中发现ActivityNotFoundException.
      原因：
      	  由于静默安装是异步的，原来的代码仅仅在子线程等待3秒后就执行启动新版本应用的动作。
      	  但是不同的apk可能由于自身大小的原因而导致安装动作并没有在3秒后真正完成。所以在启动
      	  Activity时就有可能出现找不到Activity的情况。
      解决：
          改写成以接受广播的方式来实现。apk在安装完成后系统会发出相应的广播，只要在应用完成安装
          后，再将应用的主Activity启起来就可以了。





      





