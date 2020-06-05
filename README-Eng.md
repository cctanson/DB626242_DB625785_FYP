This Related code is not execution code，it need us to verify in the verification software。This code is an embedded code in automobile system -- automobile steering system。We use verification software Frama-C to verify this code，there is an installation and terminal code：

We install in Linux 1.6  Ubuntu system，just type the code in the system terminal one by one：

​			1. apt-get/yum install frama-c

​			2. opam install depext

​			3. opam depext frama-c

​			4. opam install frama-c 

​			5. opam install alt-ergo



And then we write an annotation in Related code like Verification Code，after that, we can give Frama-C to verify，type the code in the system terminal：

​			frama-c-gui -wp -rte -val -main=steering(Main function name) steering_test(File name).c



Contact information：

IP TONG, 1213539443@qq.com or legendyetong@gmail.com

ANSON CHOU, cct.anson@gmail.com