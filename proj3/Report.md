### 使用ptherad計算pi

甲.  使用 time 函數得到的「運算時間各為多少」 

![](Pic/Aspose.Words.b843176b-538f-43ed-84ef-766c565c2881.001.png)

real :程式執行的實際時間，大約等同使用者實際等待時間，cpu 所計時的時間<br/>
user :程式執行在 user space 的時間，在 user mode 下的耗時 <br/>
sys :程式執行在 kernel space 的時間，在 kernel mode 下的耗時 <br/>

乙.   

|Num cpu |time |
| - | - |
|2 |1\.541 |
|4 |3\.479 |
|6 |5\.536 |

![](Pic/Aspose.Words.b843176b-538f-43ed-84ef-766c565c2881.002.png)


在相同後六位的精確度下，程式得到線性加速。 

丙.

upper\_buf 已知的計算結果減掉左方的面積並補上右方的面積為 0，<br/>使得在計算 lower\_buf 時可以節省重新計算需要的時間。
