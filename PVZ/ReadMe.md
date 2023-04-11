# 这是~~不正经的~~游戏说明

此游戏是借鉴~~抄袭~~了[《植物大战僵尸》](https://baike.baidu.com/item/%E6%A4%8D%E7%89%A9%E5%A4%A7%E6%88%98%E5%83%B5%E5%B0%B8/84892#:~:text=%E3%80%8A%E6%A4%8D%E7%89%A9%E5%A4%A7%E6%88%98%E5%83%B5%E5%B0%B8%E3%80%8B%E6%98%AF%E4%B8%80%E6%AC%BE%E6%9E%81%E5%AF%8C%E7%AD%96%E7%95%A5%E6%80%A7%E7%9A%84%20%E5%B0%8F%E6%B8%B8%E6%88%8F%20%E3%80%82%20%E5%8F%AF%E6%80%95%E7%9A%84%E5%83%B5%E5%B0%B8%E5%8D%B3%E5%B0%86%E5%85%A5%E4%BE%B5%EF%BC%8C%E6%AF%8F%E7%A7%8D%E5%83%B5%E5%B0%B8%E9%83%BD%E6%9C%89%E4%B8%8D%E5%90%8C%E7%9A%84%E7%89%B9%E7%82%B9%EF%BC%8C%E4%BE%8B%E5%A6%82%E9%93%81%E6%A1%B6%E5%83%B5%E5%B0%B8%E6%8B%A5%E6%9C%89%E6%9E%81%E5%BC%BA%E7%9A%84%E6%8A%97%E5%87%BB%E6%89%93%E8%83%BD%E5%8A%9B%EF%BC%8C%E7%9F%BF%E5%B7%A5%E5%83%B5%E5%B0%B8%E5%8F%AF%E4%BB%A5%E6%8C%96%E5%9C%B0%E9%81%93%E7%BB%95%E8%BF%87%E7%A7%8D%E6%A4%8D%E5%9C%A8%E5%9C%9F%E5%A3%A4%E8%A1%A8%E9%9D%A2%E7%9A%84%E6%A4%8D%E7%89%A9%E7%AD%89%E3%80%82,%E7%8E%A9%E5%AE%B6%E9%98%B2%E5%BE%A1%E5%83%B5%E5%B0%B8%E7%9A%84%E6%96%B9%E5%BC%8F%E5%B0%B1%E6%98%AF%E6%A0%BD%E7%A7%8D%E6%A4%8D%E7%89%A9%E3%80%82%2049%E7%A7%8D%E6%A4%8D%E7%89%A9%E6%AF%8F%E7%A7%8D%E9%83%BD%E6%9C%89%E4%B8%8D%E5%90%8C%E7%9A%84%E5%8A%9F%E8%83%BD%EF%BC%8C%E4%BE%8B%E5%A6%82%20%E6%A8%B1%E6%A1%83%E7%82%B8%E5%BC%B9%20%E5%8F%AF%E4%BB%A5%E5%92%8C%E5%91%A8%E5%9B%B4%E4%B8%80%E5%AE%9A%E8%8C%83%E5%9B%B4%E5%86%85%E7%9A%84%E6%89%80%E6%9C%89%E5%83%B5%E5%B0%B8%E5%90%8C%E5%BD%92%E4%BA%8E%E5%B0%BD%EF%BC%8C%E8%80%8C%E9%A3%9F%E4%BA%BA%E8%8A%B1%E5%8F%AF%E4%BB%A5%E5%90%83%E6%8E%89%E6%9C%80%E9%9D%A0%E8%BF%91%E8%87%AA%E5%B7%B1%E7%9A%84%E4%B8%80%E5%8F%AA%E5%83%B5%E5%B0%B8%E3%80%82%20%E7%8E%A9%E5%AE%B6%E5%8F%AF%E4%BB%A5%E9%92%88%E5%AF%B9%E4%B8%8D%E5%90%8C%E5%83%B5%E5%B0%B8%E7%9A%84%E5%BC%B1%E7%82%B9%E6%9D%A5%E5%90%88%E7%90%86%E5%9C%B0%E7%A7%8D%E6%A4%8D%E6%A4%8D%E7%89%A9%EF%BC%8C%E8%BF%99%E4%B9%9F%E6%98%AF%E8%83%9C%E5%88%A9%E7%9A%84%E8%AF%80%E7%AA%8D%E3%80%82)和4399上的[《美食大战老鼠》](https://my.4399.com/yxmsdzls/)将二者胡乱阉割并重组后的产物，供大家娱乐。

该游戏使用C++编写（仅使用了部分stl，编写风格更接近C），搭配使用[EasyX图形库](https://easyx.cn/)。

## 游戏玩法

该游戏类似于《植物大战僵尸》，通过收集阳关，种植植物，抵御僵尸的入侵。这里简要介绍各个植物的用法和特殊的机制。

**游戏难度仅经过初略调试**，仅供娱乐。

### 植物介绍

向日葵    <img title="" src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/向日葵1.jpg" alt="向日葵1.jpg" data-align="inline" width="50">     生产阳光

豌豆射手    <img src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/豌豆射手1.jpg" title="" alt="豌豆射手1.jpg" width="50">    发射豌豆，抵御僵尸入侵

棉花糖    <img src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/棉花糖1.jpg" title="" alt="棉花糖1.jpg" width="50">    放置在岩浆上，使得岩浆上得以种植植物。在岩浆上的棉花糖的耐久度会

逐渐减少，当减少至初始值的$\frac 14$时，会变成<img src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/棉花糖2.jpg" title="" alt="棉花糖2.jpg" width="50">需要及时补充

冰淇凌    <img src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/冰淇淋1.jpg" title="" alt="冰淇淋1.jpg" width="50">    种植在地图上已有植物的位置，使得该植物对应的冷却时间缩短

吐司面包    <img src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/吐司面包1.jpg" title="" alt="吐司面包1.jpg" width="50">    等同于《植物大战僵尸》的坚果，血厚耐啃

### 机制介绍

![background.png](C:\note\项目\PVZ\PVZ\ConsoleApplication1\素材\图片\背景\background.png)

地图资源取自《美食大战老鼠》火山岛地图《雪顶火山》。

地图格子分为普通格和岩浆格，植物无法种植在未经棉花糖<img src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/棉花糖1.jpg" title="" alt="棉花糖1.jpg" width="50">覆盖的岩浆上。

豌豆射手<img src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/豌豆射手1.jpg" title="" alt="豌豆射手1.jpg" width="50">发射的豌豆无法穿越未经棉花糖<img src="file:///C:/note/项目/PVZ/PVZ/ConsoleApplication1/素材/图片/植物/棉花糖1.jpg" title="" alt="棉花糖1.jpg" width="50">覆盖的岩浆。

## 素材来源

图片素材全部取自[爱给网](https://www.aigei.com/s?type=2d&term=album&dim=plants_v_s_zombies)，[美食大战老鼠](https://my.4399.com/yxmsdzls/)。

配乐取自《空之境界》的八个剧场版。
