### CPU temperature

甲.用 strace sensors 得知你的 CPU 的溫度在哪個檔案中 

![](Aspose.Words.6eac5ff8-ca17-4816-95b9-69bcbeb64f88.001.png)

檔案分別在以下路徑，我使用的是 mac另外安裝 archlinux 的作業系統，<br/>
但是就只能顯示兩核，所以作業用兩核演示 ”/sys/class/hwmon/hwmon2””/sys/class/hwmon/hwmon3/” 

乙.以下是sensors 執行的結果 

![](Aspose.Words.6eac5ff8-ca17-4816-95b9-69bcbeb64f88.002.jpeg)

以下是執行./waitTemp

![](Aspose.Words.6eac5ff8-ca17-4816-95b9-69bcbeb64f88.003.jpeg)

因為僅顯示兩核，所以我把其他 sensors 印出的資 訊也從檔案中抓出來顯示，<br/>
僅執行./waitTemp 表示使用預設 35 度，程式由約35.1 度下降至 35 度，<br/>
到達目標停止程式，印出的資訊除了有溫度之外，也有 sensors 執行時印出的電壓及電流。
