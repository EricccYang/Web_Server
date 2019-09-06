# A high performance Web Server
## 1.Introduction
  * 这个项目构建了一个具有并行属性，基于Linux系统的网页服务器。做这个项目的主要目的是通过造轮子来学习理解Linux系统和并行编程（In this project, a Web server with concurrence attribute based on Linux opeartion system is build. The main object of builing the project is learning both Linux system and concurrent programming by making wheels.）
## 2. Technique point 
1. 使用Pthread库实现服务器多线程运行，同时构建线程池，避免线程过量增长带来的资源损耗
1. 采用比较主流的生产者-消费者模型处理请求
1. 使用Epoll库实现事件边缘触发，同时配合非阻塞I/O感知请求，。。
1. 接受到的请求被储存在队列中，。。，通过构造基于二叉堆的定时器处理超时请求
1. 解析HTTP报头时使用了状态机理念
1. 支持 GET/POST/HEAD 请求
1. 支持动态请求和静态请求
## 3. Difficulty
1. 起初在理解事件触发和线程的协作机制时遇到困难
## 4. To Do
1. 没有实现RAII机制，存在内存泄漏的隐患，相对C而言用C++更为方便
1. eventfd也可以作为事件触发的一种实现方式，后续考虑使用eventfd进行改进，用以对比学习
1. 工业级的服务器需要日志记录，需要添加记录模块
