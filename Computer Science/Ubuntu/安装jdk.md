# 安装jdk

## 使用ppa源安装
1. 添加ppa
    ```bash
    sudo add-apt-repository ppa:webupd8team/java
    sudo apt-get update
    ```

2. 安装oracle-java-installer
    ```bash
    sudo apt-get install oracle-java8-installer
    ```
    安装器会提示你同意 oracle 的服务条款,选择 ok
    然后选择yes即可。

3. 设置系统默认jdk

    JDK8
    ```bash
    sudo update-java-alternatives -s java-8-oracle
    ```

4. 测试jdk 是是否安装成功:
    ```bash
    java -version
    javac -version
    ```