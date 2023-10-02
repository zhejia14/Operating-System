### inline assembly (AT&T style)
![](Aspose.Words.063939c6-f536-4948-a1bb-4795c38bb963.001.png)

在原程式碼當中rdtsc的指令會將資料存在 EDX:EAX，前半部分在 EDX，後半部分在 EAX，而第二行的 shift 指令將 EDX 內部資料向左移動 32 bits，使其在 rdx 的前 32 位元的部分，最後進行 or 的指令 rdx 與%0(指的是 memory)會只 有前半部分存在 memory 裡面，在 EAX 內的資料沒有存入。 

進行修改： 

![](Aspose.Words.063939c6-f536-4948-a1bb-4795c38bb963.002.jpeg)

在原本的程式碼中加入  “or %%rdx, %%rax\n\t”將 rdtsc 裡面的前半部分資 料 copy 到 rax 內，使得 rax 擁有完整的 rdtsc 資料，最後再把資料放入 memory 當中  “mov %%rax, %0”完成修改。 

![](Aspose.Words.063939c6-f536-4948-a1bb-4795c38bb963.003.png)
