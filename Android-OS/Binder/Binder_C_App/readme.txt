======关于binder demo程序的上机测试：

a. 烧写非Android系统，比如QT
    因为Android系统会自动运行ServiceManager, 为了能使用一个比较干净的系统.
b. 重新编译内核让它支持NFS，更新板上内核
    为了比较方便地做实验.
c. mount nfs, 运行service_manager, test_server, test_client
