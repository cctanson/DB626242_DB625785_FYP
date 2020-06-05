这个Related code不是执行代码，而是我们case study里需要验证的软件代码。这个代码是汽车系统中的一个嵌入式代码——汽车转向系统。我们用到验证软件Frama-C来进行代码验证，下面是安装和使用：

我们在Linux 1.6  Ubuntu 系统中安装，在系统终端依次打入一下代码即可：

​			1.apt-get/yum install frama-c

​			2.opam install depext

​			3.opam depext frama-c

​			4.opam install frama-c 

​			5.opam install alt-ergo



然后在Related code写入验证注释如Verification Code，就可以交给Frama-C去验证，在终端打入一下代码即可运行：

​			frama-c-gui -wp -rte -val -main=steering(主函数) steering_test(文件名称).c



联系方式：

IP TONG, 1213539443@qq.com or legendyetong@gmail.com

ANSON CHOU, cct.anson@gmail.com