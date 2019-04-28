打开 `/etc/fonts/conf.avail/64-language-selector-prefer.conf`，日文（Noto Sans CJK JP）的优先度高于中文简体（Noto Sans CJK SC）。日文中有很多汉字，但其渲染方式又与中文不太一样。既然在字体的选择上，日文优先于中文简体，那么部分中文字体被渲染成了日文字体而显得扭扭捏捏也就不奇怪了。

修复方案自然只需简单地把 Noto Sans CJK SC 置顶即可。最后修改的结果如下：

```
<?xml version="1.0"?>
<!DOCTYPE fontconfig SYSTEM "fonts.dtd">
<fontconfig>
        <alias>
                <family>sans-serif</family>
                <prefer>
                        <family>Noto Sans CJK SC</family>
                        <family>Noto Sans CJK TC</family>
                        <family>Noto Sans CJK JP</family>
                        <family>Noto Sans CJK KR</family>
                </prefer>
        </alias>
        <alias>
                <family>serif</family>
                <prefer>
                        <family>Noto Serif CJK SC</family>
                        <family>Noto Serif CJK TC</family>
                        <family>Noto Serif CJK JP</family>
                        <family>Noto Serif CJK KR</family>
                </prefer>
        </alias>
        <alias>
                <family>monospace</family>
                <prefer>
                        <family>Noto Sans Mono CJK SC</family>
                        <family>Noto Sans Mono CJK TC</family>
                        <family>Noto Sans Mono CJK JP</family>
                        <family>Noto Sans Mono CJK KR</family>
                </prefer>
        </alias>
</fontconfig>
```

改完后只需注销账户或者重启系统即可生效。