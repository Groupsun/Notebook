# Ubuntu安装、配置SSR客户端

最近换了个机场，但是electron-ssr真的不好用，而且已经很久没有更新了，存在很多bug，于是直接使用CLI ssr客户端吧

## 下载安装SSR

直接把repo clone下来即可

```shell
$ git clone https://github.com/ssrbackup/shadowsocksr
```

设定配置文件`/etc/shadowsocks.json`：

```shell
$ cd shadowsocksr
$ cp shadowsocksr/config.json /etc/shadowsocks.json
$ vi /etc/shadowsocks.json
```

将机场的服务器配置复制到json文件中即可，例子：

```json
{
    "server": "0.0.0.0",
    "server_ipv6": "::",
    "server_port": 8388,
    "local_address": "127.0.0.1",
    "local_port": 1080,
    "password": "m",
    "method": "aes-128-ctr",
    "protocol": "auth_aes128_md5",
    "protocol_param": "",
    "obfs": "tls1.2_ticket_auth_compatible",
    "obfs_param": "",
    "speed_limit_per_con": 0,
    "speed_limit_per_user": 0,
    "additional_ports" : {}, // only works under multi-user mode
    "additional_ports_only" : false, // only works under multi-user mode
    "timeout": 120,
    "udp_timeout": 60,
    "dns_ipv6": false,
    "connect_verbose_info": 0,
    "redirect": "",
    "fast_open": false
}
```

## 启动、停止以及重启ssr

后台启动ssr服务：

```shell
$ python local.py -c /etc/shadowsocks.json -d start
```

停止ssr服务：

```shell
$ python local.py -c /etc/shadowsocks.json -d stop
```

重启ssr服务：

```shell
$ python local.py -c /etc/shadowsocks.json -d restart
```

上面的python最好使用python3。