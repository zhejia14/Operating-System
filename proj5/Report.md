### memory order and atomic operations

![](Pic/Aspose.Words.833bdbea-d445-4bfd-b9ca-9f099c70cc9e.001.jpeg)

型號：QEMU Virtual CPU verision 2.5+<br/> 
執行環境：Linux lima-amd64 5.15.0-52-generic x86\_64 GNU/Linux<br/> 核心數：4<br/>
delay\_size = rand\_r(&rand\_seq)%a  藉由改變 a 的值來觀察： 

a=73  亂數的等待時間的範圍越大時


![](Pic/Aspose.Words.833bdbea-d445-4bfd-b9ca-9f099c70cc9e.002.jpeg)

就有較大的機率獲得較公平的結果，但是由 a＝1993 的結果發現，<br/> 
相較 a=947 並沒有比較公平，所以只是有更大的機會獲得公平的結果。 
