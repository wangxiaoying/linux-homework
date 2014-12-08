#Linux Homework 1 

#####姓名: 王笑盈 &nbsp;&nbsp; 学号: 1252885 &nbsp;&nbsp; 日期: 2014年11月

######<https://github.com/wangxiaoying/linux-homework1>
---

###题目 1

#####题目描述：一个实现ls功能的程序，并且支持不带参数和带有参数-l的两种形式

#####提交：
- 源文件：myls.cpp
- 可执行文件：myls

#####功能：
- 默认路径为当前路径
- 支持无参数
- 支持 -l 参数
- 支持多目录
- 参数先后顺序没有要求

#####运行实例:
![myls-image](https://raw.githubusercontent.com/wangxiaoying/linux-homework1/master/images/image1.png)

![myls-image](https://raw.githubusercontent.com/wangxiaoying/linux-homework1/master/images/image2.png)
	
###题目 2

#####题目描述：什么是符号链接和硬连接？试比较两者异同

#####解答：
```
linux下的链接分为硬链接和符号链接。
硬链接：由于linux下的文件是通过索引节点（Inode）来识别文件，硬链接可以认为是一个指针，指向文件索引节点的指针，系统并不为它重新分配inode。
符号链接（软链接）：软链接是一个完整的文件，而这个文件中存放的是它自己的inode号和真实的那个源文件的路径。作用在这个文件上的操作除了删除都直接转向实际指向文件，由于它是一个真实的文件所以占用磁盘空间。
相同：
1. 都是一种在共享文件和访问它的用户的若干目录项之间建立联系的一种方法。
2. 无论是硬链接文件还是符号链接文件还是原文件，更改任意一个文件都会使与它相链接的文件发生相同的改变。
不同：
1. 硬链接指向的是节点(inode),而符号链接指向的是路径(path)。
2. 硬链接不能给目录创建，并且只能在同一文件系统中的文件之间才能创建，符号链接没有这两条限制。
3. 创建硬链接后文件的链接数加1，创建符号链接文件链接数不变。
4. 在原文件被删除后，符号链接变得不可用，硬链接没有任何影响。
5. 硬链接文件与原文件拥有同一个inode号，符号链接文件与原文件的inode号不相同。
6. 在文件属性中，符号链接文件被标明是链接文件，而硬链接文件与原文件有相同的地位，不会被标明。
7. 硬链接与原文件大小相同，符号链接不同
```

###题目 3

#####题目描述：写一个程序，向一个文本中插入一段内容

#####提交：
- 源文件：insert.c
- 可执行文件：insert

#####功能：
- 运行时必须传入3个参数
- 参数一：插入位置
- 参数二：源文件名
- 参数三：目标文件名
- 源文件必须存在
- 目标文件若不存在，会自动新建
- 插入位置如果大于目标文件大小，则插入到文件末尾
- 插入位置如果小于0，则插入到文件首

#####执行：
格式如下（首行为正确情况，二三行为源文件不存在情况）：
![myls-image](https://raw.githubusercontent.com/wangxiaoying/linux-homework1/master/images/image3.png)