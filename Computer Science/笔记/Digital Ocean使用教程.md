# Digital Ocean使用教程

使用Digital Ocean搭建服务器，搭配Shadowsocks使用。开通Digital Ocean服务端的步骤如下（注册账号等都不用说了）。
1. 服务器使用位于SanFrancisco的，系统使用CentOS。
2. 进入console access远程终端的链接。
3. 第一次进入需要输入原始的账号密码，账号是root，密码会在注册邮箱中找到（在创建完droplet后发送至邮箱），输入完原始的账号密码后，系统会让你再次输入原始的密码来改变密码，所以又要再次输入三次密码，第一次是原始密码，第二，三次是修改后密码。
4. 首先安装一些必要的组件，输入下列代码运行
    ```bash
    yum install m2crypto python-setuptools
    easy_install pip
    pip install shadowsocks
    ```
5. 安装完毕后，输入
    ```bash
    vi /etc/shadowsocks.json
    ```
    创建一个新文件，然后自动进入文件内容视图后，按i进入编辑模式，输入下列代码：
    ```JSON
        {
            "server":"0.0.0.0",
            "server_port":8388,
            "local_address": "127.0.0.1",
            "local_port":1080,
            "password":"mypassword",
            "timeout":300,
            "method":"aes-256-cfb",
            "fast_open": false,
            "workers": 1
        }
    ```
    然后按esc退出编辑模式，输入
    :wq!
    保存内容。
6. 最后输入
    ```bash
    ssserver -c /etc/shadowsocks.json
    ```
    就可以成功运行了。
