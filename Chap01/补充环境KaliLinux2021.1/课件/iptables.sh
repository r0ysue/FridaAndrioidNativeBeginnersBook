#不重定向目的地址为服务器的包
iptables -t nat -A OUTPUT -d 192.168.179.1 -j RETURN  #请用你的shadowsocks服务器的地址替换$SERVER_IP
 
#不重定向私有地址的流量
iptables -t nat -A OUTPUT -d 10.0.0.0/8 -j RETURN
iptables -t nat -A OUTPUT -d 172.16.0.0/16 -j RETURN
iptables -t nat -A OUTPUT -d 192.168.0.0/16 -j RETURN
 
#不重定向保留地址的流量,这一步很重要
iptables -t nat -A OUTPUT -d 127.0.0.0/8 -j RETURN
 
#重定向所有不满足以上条件的流量到redsocks监听的12345端口
iptables -t nat -A OUTPUT -p tcp -j REDIRECT --to-ports 12345 #12345是你的redsocks运行的端口,请根据你的情况替换它
