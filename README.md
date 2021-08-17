GMP
==========
# Global Multi-Probing for Maximum Inner Product Search with Adaptive Early Termination
-----------------------------------------------------------------------------------------------------------------
### Introduction
This is a source code for the algorithm described in the paper **[Global Multi-Probing for Maximum Inner Product Search with Adaptive Early Termination (PVLDB 2022)]**. We call it as GMP project. 

### Compilation

GMP project is written by C++ and is simple and easy to use. 

#### Installation

cd build
cmake ..
(if cmake successfully the Makefile is in build directory)
make (option parameters)

#### Execution

(the excutable file is in build directory)
mips.exe

### Usage
#### Command Usage

-------------------------------------------------------------------
> mips.exe datasetName project_path k c
------------------------------------------------------------------- 
(the first parameter specifies the procedure be executed)

------------------------------------------------------------------- 
#### Parameter explanation

- datasetName  : dataset name
- project_path : current project directory name
- k            : 1-100, an integer, the number of returned points
- c            : 0-1, a float number, the approximate ratio
-------------------------------------------------------------------

FOR EXAMPLE, YOU CAN RUN THE FOLLOWING CODE IN COMMAND LINE AFTER BUILD ALL THE TOOLS:

```
> mips.exe audio C:\mips 50 0.5
```
#### Dataset

In our project, the format of the input file (such as `audio.data`, which is in `float` data type) is the same as that in [LSHBOX](https://github.com/RSIA-LIESMARS-WHU/LSHBOX). It is a binary file but not a text file, because binary file has many advantages. The binary file is organized as the following format:

>{Bytes of the data type} {The size of the vectors} {The dimension of the vectors} {All of the binary vector, arranged in turn}

For your application, you should also transform your dataset into this binary format.

You can get the sample dataset `audio.data` from [here](http://www.cs.princeton.edu/cass/audio.tar.gz), if the link is invalid, you can also get it from [data](https://github.com/RSIA-LIESMARS-WHU/LSHBOX-sample-data).

For other dataset, you can get the raw data from following websites: [Netflix](https://academictorrents.com/details/9b13183dc4d60676b773c9e2cd6de5e5542cee9a), [MovieLens](https://grouplens.org/datasets/movielens/), [MNIST](http://yann.lecun.com/exdb/mnist/index.html), [Cifar](http://www.cs.toronto.edu/~kriz/cifar.html), [Trevi](http://phototour.cs.washington.edu/patches/default.htm), [NUS](https://pan.baidu.com/share/init?surl=kVKfXFx)(Extraction code: hpxg), [YahooMusic](https://webscope.sandbox.yahoo.com/catalog.php?datatype=r), [GIST](http://corpus-texmex.irisa.fr/), [TinyImages](https://hyper.ai/tracker/download?torrent=6552).


#### Result
The experiment result is saved in current directory "result", including:
Running_result_final.txt
GMP_result.csv


## Acknowledgement
GMP project is developed by referring to LSHBOX (https://github.com/RSIA-LIESMARS-WHU/LSHBOX). Great appreciation to the contributors of LSHBOX. 

## Reference 
**[Global Multi-Probing for Maximum Inner Product Search with Adaptive Early Termination (PVLDB 2022)]**

