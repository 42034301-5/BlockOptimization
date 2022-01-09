# 基本块优化
基于有向无环图的编译中间代码基本块优化

编译（请使用 g++ 9 以上的版本）：
```
g++ -std=c++17 DAGOpt.cpp
```
运行：
```
./a.exe blk.json result.json
```
其中```blk.json```为切割好的基本块的文件名，```result.json```为输出文件名。
<br><br>
可执行文件的输出包括：
- 一个json文件，包含局部优化后的各基本块信息
- DAG.txt，包含输入json文件中所有基本块对应的DAG数据结构展示
<br><br>



输入输出文件的格式和支持的中间代码指令参见[OptimizerExpDoc](https://github.com/42034301-5/OptimizerExpDoc)