# Coursera的PAC文件添加问题

Coursera的服务器是在境外，因此在国内如果直连的话速度感人。之前尝试了直接添加了Coursera的域名进ss的PAC文件中希望在访问Coursera的时候能走ss通道，但是发现缓慢依旧，并且某些元素无法加载，推测是这些元素提供的服务器不是域名服务器。

用chrome的开发者工具检查了一下Coursera视频页的文件来源，主要在coursera.org和cloudfront.net域名下，cloudfront.net是亚马逊AWS的CDN加速域名，也需要走代理加速。

因此，为了提升访问Coursera的速度，应该在PAC文件中添加如下条目：
```JSON
  "||cloudfront.net",
  "||coursera.org",
```