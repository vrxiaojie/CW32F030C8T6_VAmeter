
<span style="font-size:20px;font-family:Microsoft YaHei">
    
# 1 简介
&ensp;&ensp;作为电子爱好者的你我，一定用万用表测量过电压和电流，以判断电路是否正常工作.但你是否好奇，测量电压电流背后的原理是什么？能不能自己做一个电压电流表呢？本开源工程就手把手地教你从硬件设计原理到软件代码、从数据库搭建到数据可视化展示，一步步做一个基于立创地文星（CW32G030C8T6）的多功能物联网电压电流表.</font>

&ensp;&ensp;首先上几张实物图.各位大佬们可以基于本项目，继续改进优化PCB布局、软件设计~

![图1 实物正面.jpg](https://image.lceda.cn/oshwhub/8ae4f4c6c7914a5b84eab371f38059fc.jpg)

<body>
	<p align="center">图1 实物正面</p>
</body>


![图2 实物背面.jpg](https://image.lceda.cn/oshwhub/db8b4a47c15247e5995b97c887314a9c.jpg)

<body>
	<p align="center">图2 实物背面</p>
</body>

# 2 硬件设计及原理
## 2.1 供电电路
&ensp;&ensp;电源拓扑图如图3所示.

![图3电源拓扑图.jpg](https://image.lceda.cn/oshwhub/415c5566745e481a8509559cce55a7b1.jpg)
<body>
	<p align="center">图3 电源拓扑图</p>
</body>
&ensp;&ensp;供电部分使用了一节18650锂电池，充电管理芯片用的是TC4056A，最大可支持到1A充电电流，电路设计如图4所示.


![图4 锂电池.jpg](https://image.lceda.cn/oshwhub/d81218e8ddd94a8d9c8b709707b8cba0.jpg)
<body>
	<p align="center">图4 充电管理电路</p>
</body>

&ensp;&ensp;如图5所示，DCDC升压电路部分，使用的是MT3608B，其在电流小于100mA时的效率约92%.输出电压是使用电阻分压反馈方式，VOUT=(1+R2/R1) * VREF.手册里写VREF=0.6V，我取R2=91KΩ，R1=13KΩ得到VOUT=4.8V.这里两个分压电阻大一点好，这样流过它们的电流小，它们所耗的功率也会变小.这里的电感4.7uH是按芯片手册来的，建议选择一个等效直流电阻更低的电感，这样也能提高效率.这里的续流二极管必选肖特基二极管，考虑到整机电流不大，用SS14就行.


![图5 dcdc.jpg](https://image.lceda.cn/oshwhub/0ca0fafd4e3d431892f3386f61fac4e4.jpg)
<body>
	<p align="center">图5 DC-DC升压电路</p>
</body>

&ensp;&ensp;降压至3.3V的LDO为RS3236-3.3，选用它是因为其有EN引脚，可用CW32的GPIO控制LDO启停，此外它还有极低的待机电流(0.01uA)，能让整机静态电流再降一些.LDO电路设计见图6.


![图6 ldo.jpg](https://image.lceda.cn/oshwhub/5fd8f2a25ccc4a0eb672f1b808281eba.jpg)
<body>
	<p align="center">图6 LDO降压电路</p>
</body>

## 2.2 MCU选型——为什么选CW32

&ensp;&ensp;测量电压和电流，实际上都是在采集电压数据，而电压值是模拟量，要转换成程序能读取的数字量就要用到ADC(模数转换)，立创地文星开发板是使用的CW32F030C8T6，拥有12位精度的高速ADC，有多种Vref参考电压：1.5V、2.5V、VDD、ExRef（PB00）引脚电压.而STM32只有一个VDD作为参考电压，考虑到精度问题，自然是可选的Vref多一点好.CW32用户手册对ADC的介绍如图7所示. 


![图7 ADC介绍.jpg](https://image.lceda.cn/oshwhub/468ea40526524a299e293fbd51b62d3e.jpg)
<body>
	<p align="center">图7 CW32数据手册对ADC介绍</p>
</body>

&ensp;&ensp;此外，还考虑到本项目使用了TFT屏幕、ESP8266、按键控制、ADC采集，所需要的IO口数量较多，地文星开发板提供了32个IO引出，很够用了.

## 2.3 ADC电路
&ensp;&ensp;ADC,是Analog-to-Digital Converter的缩写，指模/数转换器，是将连续变量的模拟信号转换为离散的数字信号的器件.

&ensp;&ensp;ADC引脚功能说明如下表所示


|端口-ADC通道|功能|
|:---:|:---:|
|PBA7-ADC_IN7 | 电池电压测量|
|PB00-ADC_IN8 |电压测量校准|
|PB01-ADC_IN9  |电压测量0~3V|
|PB10-ADC_IN11| 电压测量0~31V|
|PB11-ADC_IN12 |电流测量0~1A|
### 2.3.1电压采样电路
&ensp;&ensp;电压采样电路设计图如图8所示，选取Vref为内置1.5V电压.


![图8 电压采样电路.jpg](https://image.lceda.cn/oshwhub/43a7b2a3b2ac4c58955b4d9a68dadcf8.jpg)
<body>
	<p align="center">图8 电压采样电路</p>
</body>

&ensp;&ensp;大量程这边使用一个200k和10k电阻分压，采集10k电阻上的电压值，则最大量程计算方法为:

$$
U_{adc11max}= \frac {1.5}{\frac{10k}{10k+200k}}=31.5V
\tag {1}
$$

&ensp;&ensp;取整数31V，当电压为31V时，算一下电阻R2和R5上的功率：

$$
P_{R5max}=  \frac{(31\times \frac{10k}{10k+200k}) ^{2}}{10k}\approx 0.218mW
\tag {2}
$$

$$
P_{R2max}=  \frac{(31\times \frac{200k}{10k+200k}) ^{2}}{200k}\approx 4.36mW
\tag {3}
$$

&ensp;&ensp;所以R2和R5选择0603的封装（100mW）都绰绰有余，按个人手头上有的器件来选贴片电阻的封装即可.

&ensp;&ensp;小量程这边使用了两颗10kΩ ±0.1%电阻分压，最大量程计算方式与 **式(1)** 相同，可算得量程为0~3V.

&ensp;&ensp;后面在软件部分实现了自动换挡功能，实现了小电压高精度测量、大电压经校准后也能有很高的精度.

### 2.3.2 电流采样电路
由
$$
I_s = \frac{V_{REF}}{4096} \times \frac{1}{R_{sa}} \tag{4}
$$
可知，在参考电压$V_{REF}$一定的情况下，采样电阻$R_{sa}$越大，对应的$I_s$越小.因为本人经常测量毫安级的电流，自然希望单位ADC值所对应的电流越小越好.故选用2512封装的200mΩ电阻，功率为2W.

$$
I_{Rmax} = \sqrt{\frac{P_{max}}{R_{sa}}} = \sqrt{\frac{2}{0.2}}\approx 3.16A
\tag{5}
$$
&ensp;&ensp;由 **式(5)** 知，200mΩ电阻能采集的最大电流为3.16A，大于设计的采集电流1A.此时单位ADC值对应的电流为

$$
I_s = \frac{1.5V}{4096} \times \frac{1}{0.2Ω} \approx 1.83mA \tag{6}
$$

符合设计需求.

&ensp;&ensp;电流采样电路设计如图9所示.

![图9 电流采样电路.jpg](https://image.lceda.cn/oshwhub/b28d0fd4320d471994ad5568a3a5ad3c.jpg)
<body>
	<p align="center">图9 电流采样电路</p>
</body>

### 2.3.3 电池电压采样
&ensp;&ensp;电池电压采样电路设计见图10.使用两个10KΩ ±0.1%的电阻分压.选用内部参考电压2.5V. 


![图10 电池电压.jpg](https://image.lceda.cn/oshwhub/2c8d5c5f3d4f4ccb9c9e6c23b73bf8b0.jpg)
<body>
	<p align="center">图10 电池电压测量电路</p>
</body>

### 2.3.4 TL431电压校准电路(可选)
&ensp;&ensp;在本项目中还使用了一个TL431做电压校准电路，用于提供2.5V的基准电压，虽然CW32内置了2.5V的电压基准，理论上无需外部基准，但可用TL431来学习电压基准芯片的应用原理. 


![图11 TL431原理图.jpg](https://image.lceda.cn/oshwhub/8bc91e0606114b16b16fb041d7c79707.jpg)
<body>
	<p align="center">图11 TL431原理图</p>
</body>

&ensp;&ensp;如图11所示，TL431的核心是一个运放，在电路中充当比较器.芯片内部有一个Vref电压（约为2.5V），作用在比较器的反相端.比较器的同相端会输入一个电压给REF，当这个电压大于Vref时，比较器输出高电平，使能三极管，使CATHODE（阴极）端与 ANODE（阳极）端导通，此时，若REF和CATHODE处于同一电位（连接在一起），则REF处的电位被拉低，当REF处的电位被拉低至低于Vref时，比较器输出低电平，三极管关断，REF处的电位回升，当高于Vref时，继续执行以上描述，如此循环.由于硬件的响应速度是极其快的.所以REF处的电压几乎等于Vref.

&ensp;&ensp;TL431电路设计见图12.

![图12 TL431电路图.jpg](https://image.lceda.cn/oshwhub/77fccbfbf0764b03b6bbc286afc97e46.jpg)
<body>
	<p align="center">图12 电压校准电路</p>
</body>

## 2.4 按键电路
&ensp;&ensp;CW32的I/O口内部可以配置上下拉电阻，在芯片外围的按键控制电路则无需配置.按键一端接入MCU的I/O上，另一端接地.按键按下，I/O被拉低，在代码编写时去检测下降沿然后进中断即可，代码部分会在软件设计章节(第3节)说明.按键电路设计如图13所示.


![图13 按键电路.jpg](https://image.lceda.cn/oshwhub/a4345b240ab44b58a9f6172571b17be3.jpg)
<body>
	<p align="center">图13 按键控制电路</p>
</body>

各个IO端口对应的按键操作逻辑如下表所示

|IO端口|操作逻辑|
|---|:---:|
|PA12|上|
|PA8|下|
|PA9|左|
|PA11|右|
|PA10|OK|

## 2.5 ESP8266电路
&ensp;&ensp;既然本项目是“物联网”电压电流表，则必须要用WIFI去连接网络，上传数据.因此使用了乐鑫的ESP8266，其核心是ESP-12F+配套转接板，在tb上一套约6元，用转接板是为了方便拔插、重复使用的，如图14所示.

&ensp;&ensp;另外，如果你和我一样要用CW32去控制ESP8266的启停，则需要把转接板上的一个10K电阻去掉，具体位置见图14.


![图14 ESP8266.jpg](https://image.lceda.cn/oshwhub/df758f3e60e9400c98cb2cd5da97bb3d.jpg)
<body>
	<p align="center">图14 ESP8266模块及转接板</p>
</body>

&ensp;&ensp;ESP8266的电路设计见图15，将8266的串口与CW32的PB8、PB9相连，并引出排针作为外部烧录接口.此外可通过跳线帽使GPIO0接地，以切换下载或者调试模式，无需外部连线.

![图15 ESP8266电路.jpg](https://image.lceda.cn/oshwhub/ef5b4db5d3b74690a2edf8faa7284358.jpg)
<body>
	<p align="center">图15 ESP8266电路设计</p>
</body>

## 2.6 TFT屏幕
&ensp;&ensp;本项目使用的TFT屏幕为1.8寸彩屏，分辨率128*160，驱动为ST7735.使用SPI通信，引脚定义及功能如下表所示：
|引脚|功能|
|:--:|:--:|
|SCL |SPI时钟, 对应上位机SPI的SCK|
|SDA |SPI数据输入, 对应上位机SPI的MOSI|
|RES |重启, 低电平有效, 工作时处于高电平|
|DC |命令模式和数据模式切换位, 低电平为命令模式, 高电平为数据模式|
|CS |片选信号, 对应上位机SPI的CS|
|BL |背光, 高电平亮, 低电平灭,可PWM调光|

&ensp;&ensp;图16为TFT屏幕外观，请注意与商家确认是否为ST7735驱动，否则需要你自己适配代码.


![图16 TFT.jpg](https://image.lceda.cn/oshwhub/4ebc395537ad4468a7ee48095a22803e.jpg)
<body>
	<p align="center">图16 TFT屏幕外观</p>
</body>

## 2.7 SHT40温湿度采集（可选）
&ensp;&ensp;本着“多功能”的想法，本项目可选择性地增加一个温湿度传感器，以通过屏幕监测温湿度，将这些数据发送至云端数据库.

&ensp;&ensp;SHT40原理以及代码实现可参考我之前的开源工程: [基于STM32和SHT40的桌面低功耗可充电温湿度计]( https://oshwhub.com/vrxiaojie/temperature-and-humidity-meter-7508164a )  在此就不再赘述.

# 3 软件设计

## 3.1 简介
&ensp;&ensp;OK,终于来到软件代码部分了.这里有95%以上的核心实现代码是我自己一点点编写起来的，也是第一次自己做了一个操作菜单，其中的逻辑关系还是比较复杂的.编写工程代码前后用时将近7天.

程序API文档可在线查看：[基于CW32的电压电流表-DOC](https://wjj0227.github.io/CW32F030C8T6_VAmeter/files.html)

&ensp;&ensp;代码主要部分包括
- 菜单设计
- TFT屏幕显示图像和文字
- ESP8266 WIFI、MQTT设置
- 电压电流表实现
- SHT40 温湿度获取及显示
- 系统设置
- ADC、定时器及按键控制

&ensp;&ensp;限于开源工程文档的篇幅，恕无法面面俱到讲解，如果你对软件实现感兴趣，可以下载**附件**中的工程文件.几乎每个函数我都有写说明，一些重要的语句也有注释，如有不理解的或者代码改进建议，欢迎在评论区中留言讨论.

## 3.2 菜单设计
&ensp;&ensp;菜单逻辑上采用两级菜单，主菜单用来选择功能，如连接WIFI、电压电流表等，放在屏幕左侧；而次级菜单用来选择主功能下的分支，放在屏幕右侧.菜单层级图如图17所示.


![图17 menu.jpg](https://image.lceda.cn/oshwhub/2819916ae8c0479d8b7d98debfc5ae25.jpg)
<body>
	<p align="center">图17 菜单层级框图</p>
</body>
 
&ensp;&ensp;菜单相关的函数均放在了`menu.c`源文件下.`Show_Status_Bar`函数用以显示当前电量、WIFI连接状态、ESP8266连接状态、MQTT连接状态.核心是调用了TFT屏幕的绘图函数`showimage_16`，该函数可在`GUI.c`源文件中查看.其中，被绘制的图片又是由图片取模工具得到的，转换后的十六进制数组放在了`Picture.c`文件中

```c
void Show_Status_Bar()
{
	//最左侧放电池电量
	uint16_t bat_volume = Battery_Volume();
	if(bat_volume>=61 && bat_volume<=100)
	{
		Gui_DrawFont_Num16(0,0,GREEN,BLACK,bat_volume/10);
		Gui_DrawFont_Num16(8,0,GREEN,BLACK,bat_volume%10);
	}
	else if(bat_volume>=21 && bat_volume<=60)
	{
		Gui_DrawFont_Num16(0,0,YELLOW,BLACK,bat_volume/10);
		Gui_DrawFont_Num16(8,0,YELLOW,BLACK,bat_volume%10);
	}
	else if(bat_volume>0&& bat_volume<=20)
	{
		Gui_DrawFont_Num16(0,0,RED,BLACK,bat_volume/10);
		Gui_DrawFont_Num16(8,0,RED,BLACK,bat_volume%10);
	}
	Gui_DrawFont_GBK16(16,0,WHITE,BLACK,"%");
	
    //右数第一个放WIFI状态
    switch(WIFI_Status)
    {
    case 0:
        showimage_16(WIFI_Disconnected_Icon,144,0);
        mqtt_status = 0;
        break;
    case 1:
        showimage_16(WIFI_Connected_Icon,144,0);
        break;
    }

    //右数第二个放ESP8266连接状态
    switch(ESP8266_Status)
    {
    case 0 :
        showimage_16(ESP8266_ERROR,124,0);
        break;
    case 1 :
        showimage_16(ESP8266_OK,124,0);
        break;
    }
    //右数第三个放MQTT连接状态
    switch(mqtt_status)
    {
    case 0:
        showimage_16(Mqtt_Error_Icon,104,0);
        break;
    case 1:
        showimage_16(Mqtt_OK_Icon,104,0);
        break;
    }
}
```

&ensp;&ensp;显示主菜单和次级菜单的函数分别为`Show_Main_Menu`和`Show_Sub_Menu`.预先将按钮的名称存放于`char* main_menu_button[]`和`char* sub_menu_button[5][5]`数组中，这样只需根据菜单的索引值即可绘制某个位置的按钮名称.

```c
void Show_Main_Menu()
{
    //画分割线
    Gui_DrawLine(0,20,160,20,GREEN);
    Gui_DrawLine(80,20,80,128,GREEN);
    //显示状态栏
    Show_Status_Bar();
    //渲染主菜单
    for(uint8_t i = 0; i<5; i++)
    {
        uint8_t x = 0,y = 22+i*20;
        Gui_DrawFont_GBK16(x,y,WHITE,BLACK,main_menu_button[i]);
    }
    Choose_Main_Function();  //高亮选中主菜单
	Show_Sub_Menu(); //同时显示当前次级菜单
	if(in_sub_menu_flag == 1) //若在次级菜单，则高亮选中次级菜单的功能
		Choose_Sub_Function();
}
```
```c
void Show_Sub_Menu()
{
	Lcd_Part_Clear(81,22,160,128,BLACK);
	for(uint8_t i = 0; i<=sub_menu_maxidx[main_menu_index]; i++)
    {
        uint8_t x = 81,y = 22+i*20;
        Gui_DrawFont_GBK16(x,y,WHITE,BLACK,sub_menu_button[main_menu_index][i]);
    }
}
```

&ensp;&ensp;有了整体框架后，就要追究细节问题了，该怎样显示点击上下左右按键后，当前所选择的按钮呢？这就用到了下面四个函数，它们是用来绘制主、次级菜单按钮被选中与恢复未被选中状态的.

```c
/*次级菜单按钮被选中*/
void Choose_Sub_Function()
{
	uint8_t x = 81,y=40;
    Gui_DrawLine(x,y+sub_menu_index*20,x+75,y+sub_menu_index*20,ORANGE);
}
/*主菜单按钮被选中*/
void Choose_Main_Function()
{
    uint8_t x = 0,y=40;
	Gui_DrawLine(x,y+main_menu_index*20,x+75,y+main_menu_index*20,LIGHTBLUE);
}

//次级菜单 从被选中状态恢复成未被选中状态
void Restore_Sub_Menu_Button(uint8_t idx)
{
	uint8_t x = 81,y = 40;
	Gui_DrawLine(x,y+idx*20,x+75,y+idx*20,BLACK);
}

//主菜单 从被选中状态恢复成未被选中状态
void Restore_Main_Menu_Button(uint8_t idx)
{
	uint8_t x = 0,y = 40;
	Gui_DrawLine(x,y+idx*20,x+75,y+idx*20,BLACK);
}
```

&ensp;&ensp;到这步为止，好像还差一个东西，那就是执行相应按钮所对应的功能，即按下“确认”键后，进到功能所在界面.使用的函数为`Run_Function`.所有的函数也都存在一个指向函数地址的二维数组 `void (*FUN[5][5])()`里面，要调用的时候只需传入对应的索引值即可.

```c
/*存放各个运行函数的数组*/
void (*FUN[5][5])() = {
{Launch_VA_Meter,Launch_VA_Meter},//电压电流
{SHT40_Show_Temp_Humid},//温湿度计
{Connect_WIFI,Network_info},//WIFI
{MQTT_Setup},//MQTT
{System_Info,ESP8266_Reset,MCU_Sleep,Brightness_Setup}};//系统设置
```
```c
void Run_Function()
{
    in_progress_flag = 1;
	Lcd_Clear(BLACK);
	if(main_menu_index==0)  //针对电压电流表菜单单独设置，用于传参
	{
		FUN[main_menu_index][sub_menu_index](sub_menu_index);
	}
		
	else
	{
		FUN[main_menu_index][sub_menu_index]();
	}
}
```

## 3.3 WIFI连接&MQTT连接
&ensp;&ensp;与WIFI连接有关的函数放在了`esp8266_cmd.c`文件下，配置WIFI名和密码需要在`main.h`文件的宏定义中配置.


&ensp;&ensp;向ESP8266发送命令，主要靠的是ESP8266的官方MQTT AT固件.AT指令是非常好用的，只需要在串口发送命令即可.例如在电脑给ESP8266串口发送`AT`，ESP8266会返回`OK`，所有的AT指令集参见[官方文档](https://docs.espressif.com/projects/esp-at/zh-cn/release-v2.2.0.0_esp8266/AT_Command_Set/index.html).

&ensp;&ensp;那么用程序让CW32在串口给ESP8266发送指令，即可实现与电脑串口向ESP8266发指令一样的功能.实现发送指令的函数为`ESP8266_SendCmd`，其底层的串口发送函数为`USART_SendString`.

&ensp;&ensp;在用户层，只需调用连接WIFI函数`Connect_WIFI`即可完成WIFI的连接.

```c
void Connect_WIFI() {
	GPIO_WritePin(ESP8266_EN_PORT,ESP8266_EN_PIN,GPIO_Pin_SET); //开启ESP8266
	delay_ms(100);
	if(GPIO_ReadPin(ESP8266_PORT,ESP8266_PIN)==GPIO_Pin_SET)
	{
		ESP8266_Status = 1;
	}
	if(ESP8266_Status == 0)
	{
		Gui_DrawFont_GBK16(8,72,RED,YELLOW,"ESP8266 Not Found");
		delay_ms(250);
		return ;
	}
    if(WIFI_Status==0) {
		delay_ms(200);
        char *t = malloc(100);
        strcat(t,"AT+CWJAP=\"");
        strcat(t,WIFI_SSID);
        strcat(t,"\",\"");
        strcat(t,WIFI_PASSWORD);
        strcat(t,"\"\r\n");
		
        Gui_DrawFont_GBK16(0,0,WHITE,BLACK,"WIFI Connecting...");
		ESP8266_SendCmd((uint8_t *)"AT+CWMODE=1,0\r\n",(uint8_t *)"OK");
        if(ESP8266_SendCmd((uint8_t *)t,(uint8_t *)"CONNECTED"))
        {
            WIFI_Status = 1;
			FUN[2][0] = Disconnect_WIFI;
			sub_menu_button[2][0]="断开WIFI  ";
			
            Gui_DrawFont_GBK16(0,16,GREEN,BLACK,"WIFI Connected!");
            Gui_DrawFont_GBK16(0,32,WHITE,BLACK,strcat_new("SSID:",WIFI_SSID));
			ESP8266_Last_Status = 1;
            free(t);
            connect_retry_cnt = 0;
			delay_ms(200);
			Lcd_Clear(BLACK);
			Show_Main_Menu();
			in_progress_flag = 0;
            return ;
        }
        else
        {
            free(t);
            WIFI_Status = 0;
            connect_retry_cnt++;
            switch(connect_retry_cnt)
            {
            case 1:
                Gui_DrawFont_GBK16(0,16,RED,BLACK,"Retrying...[1]");
                Connect_WIFI();
                break;
            case 2:
                Gui_DrawFont_GBK16(0,16,RED,BLACK,"Retrying...[2]");
                Connect_WIFI();
                break;
            default:
                connect_retry_cnt=0;
				ESP8266_SendCmd((uint8_t *)"AT+CWMODE=0,0\r\n",(uint8_t *)"OK");
                Gui_DrawFont_GBK16(0,16,RED,BLACK,"WIFI Not Connected!");
				delay_ms(200);
				Lcd_Clear(BLACK);
				Show_Main_Menu();
				in_progress_flag = 0;
                break;
            }
            return ;
        }
    }
}
```

&ensp;&ensp;限于篇幅，网络信息查询以及断开WIFI的函数请自行在源文件中查看，原理类似.

&ensp;&ensp;连接至WIFI后，就可以连接MQTT了.首先要在`mqtt.h`头文件的宏定义中配置好MQTT连接的相关参数，具体的配置方法在4.2.3节中讲.

&ensp;&ensp;MQTT发布消息函数为`MQTT_Publish`，函数传入的参数是字符型数据数组，使用`AT+MQTTPUB`指令，经过一系列的字符串拼接操作，发送出去.
```c
void MQTT_Publish(char *data)
{
	if(mqtt_status==1)
	{
		char buffer[400] = {0};
		strcpy(buffer,"AT+MQTTPUB=0,\"");
		strcat(buffer,MQTT_TOPIC);
		strcat(buffer,"\",\"");
		strcat(buffer,data);
		strcat(buffer,"\",0,0\r\n");
		ESP8266_SendCmd((uint8_t*)buffer,(uint8_t*)"OK");
		free(buffer);
	}
}
```

## 3.4 电压电流表
&ensp;&ensp;与电压电流表功能相关的函数都在`va_meter.c`文件下.`Launch_VA_Meter`函数是启动电压电流表的入口函数，这里主要是在屏幕上绘制数值需要动点小心思处理一下，采用了取尾数的方式把浮点数转为字符串.

&ensp;&ensp;这个函数里包含了两种模式：图形模式和数字模式，由`va_meter_style`变量控制模式选择.

```c
void Launch_VA_Meter(uint8_t idx)
{
    Lcd_Clear(BLACK);
    Init_VAmeter_ADC();
	key_flag[2]=0;
	uint8_t send_wait = 100;
	double mqtt_volt,mqtt_curr;
    //初始化不同样式的界面
    switch(idx)
    {
    case 0:
        Gui_DrawFont_GBK16(40,0,LIGHTBLUE,BLACK,"电压电流表");
        Gui_DrawLine(0,18,160,18,WHITE);
        Gui_DrawLine(0,111,160,111,WHITE);
        time = 2;
        va_meter_style = 0;
        break;
    case 1:
        Gui_DrawFont_GBK16(40,0,LIGHTBLUE,BLACK,"电压电流表");
        Gui_DrawFont_Num32_2(64,24,YELLOW,BLACK,0); //显示电压的小数点
        Gui_DrawFont_GBK16(144,38,YELLOW,BLACK,"V"); //显示电压的单位
        Gui_DrawFont_Num32_2(96,80,YELLOW,BLACK,0); //显示电流的小数点
        Gui_DrawFont_GBK16(144,80,YELLOW,BLACK,"m"); //显示电压的单位 m
        Gui_DrawFont_GBK16(144,96,YELLOW,BLACK,"A"); //显示电压的单位 A
        va_meter_style = 1;
        break;
    }

    while(key_flag[2]==0)  //向左按键未被按下，一直在循环内
    {
        ADC_GET();
		
		//处理电压数据
		if(adc_value[0]>=4090)  //当电压值大于3V时，换挡到0~31V
		{
			voltage_value = (adc_value[1]/4095.0 * 1.5 * 21)-0.075;   //参考电压是1.5V，分压比1:20 经校准比正常值高0.075V，故减去0.075
			Gui_DrawFont_GBK16(128,0,RED,BLACK,"3V");
		}
		else	//电压值小于3V时，换挡到0~3V
		{
			voltage_value = (adc_value[0]/4095.0 * 1.5 * 2);	//参考电压是1.5V，分压比1:1 
			Gui_DrawFont_GBK16(128,0,GREEN,BLACK,"3V");
		}
		if(voltage_value <=0.3) //当数值为0.3以下时，视为ADC误差，将值置为0，本次采集数据无效
            voltage_value = 0.0;
		mqtt_volt = voltage_value;
        voltage_value *= 100;	//处理以显示小数点后2位
        for(uint8_t i = 0; i<4; i++)
            voltage_num[3-i]=(int)(voltage_value/(pow(10,i))) %10;
		
		//处理电流数据
        current_value = ((adc_value[2]/4095.0 * 1.5)/0.2 * 1000)-10.8;   //参考电压是1.5V，采样电阻0.2ohm，电流单位mA，经校准比理论值高10.8mA
		if(current_value < 0)
			continue;
		mqtt_curr = current_value;
		current_value *= 10;  	//处理以显示小数点后1位
		for(uint8_t i = 0; i<4; i++)
            current_num[3-i]=(int)(current_value/(pow(10,i))) %10;
		//MQTT发送间隔  4-->-约等于0.3s
		send_wait++;
		if(send_wait>4)
		{
			VAmeter_Mqtt_Send_Data(mqtt_volt,mqtt_curr);
			send_wait=0;
		}
		
        switch(va_meter_style)
        {
        case 0:
            for(uint8_t i = 0; i<2; i++)
			{
                Gui_DrawFont_Num16(i*8,112,GREEN,BLACK,voltage_num[i]);
			}
            Gui_DrawFont_GBK16(16,112,ORANGE,BLACK,".");
            for(uint8_t i = 2; i<4; i++)
            {
                Gui_DrawFont_Num16(8+i*8,112,GREEN,BLACK,voltage_num[i]);
            }
            Gui_DrawFont_GBK16(40,112,ORANGE,BLACK,"V");
			
			for(uint8_t i = 0; i<3; i++)
            {
                Gui_DrawFont_Num16(60+i*8,112,BLUE,BLACK,current_num[i]);
            }
			Gui_DrawFont_GBK16(84,112,ORANGE,BLACK,".");
            Gui_DrawFont_Num16(92,112,BLUE,BLACK,current_num[3]);
			Gui_DrawFont_GBK16(100,112,ORANGE,BLACK,"mA");
			
            Draw_Value_Line();
            break;
        case 1:
            //电压
            for(uint8_t i = 0; i<2; i++)
                Gui_DrawFont_Num32(i*32,24,ORANGE,BLACK,voltage_num[i]);
            for(uint8_t i = 2; i<4; i++)
            {
                Gui_DrawFont_Num32(16+i*32,24,BLUE,BLACK,voltage_num[i]);
            }
            //电流
            for(uint8_t i = 0; i<3; i++)
            {
                Gui_DrawFont_Num32(i*32,80,ORANGE,BLACK,current_num[i]);
            }
            Gui_DrawFont_Num32(112,80,BLUE,BLACK,current_num[3]);
            break;
        }
        delay_ms(5);
    }
	Lcd_Clear(BLACK);
	Show_Main_Menu();
	in_progress_flag = 0;
	key_flag[2]=0;
}
```

图形模式下，画图的函数为`Draw_Value_Line`.电压是用曲线图表示，电流是柱状图.

```c
void Draw_Value_Line()
{
    if(time==158)
    {
        time=2;
        Gui_DrawLine(2,20,2,110,BLACK);
    }

    voltage_value=(int)voltage_value/10;
	current_value=(int)current_value/10;
    //画图范围纵坐标20-110，横坐标2-158
    if(voltage_value>MAX_VOLTAGE_Y*10 || current_value>MAX_CURRENT_Y)
        return ;
	
	//先绘制电流柱状图，如果和电压曲线图颠倒绘制顺序，则会遮住曲线图
	Gui_DrawLine(time,110-(int)(90*current_value/MAX_CURRENT_Y),time,110,BLUE);
	
	//再绘制电压曲线图
    if(time==2)
	{
        Gui_DrawPoint(time,110-(int)(90*voltage_value/10/MAX_VOLTAGE_Y),GREEN); 
	}
    else
    {
        Gui_DrawLine(last_time,110-(int)(90*last_voltage_value/10/MAX_VOLTAGE_Y),time,110-(int)(90*voltage_value/10/MAX_VOLTAGE_Y),GREEN); 	//voltage
	}

    Gui_DrawLine(time+1,20,time+1,110,BLACK); //擦除下一时刻的值
    time++;
    last_voltage_value = voltage_value;
    last_time = time;
}
```

&ensp;&ensp;还有一个函数`VAmeter_Mqtt_Send_Data`，是发送电压、电流和功率数据到MQTT消息队列的，在连接WIFI、MQTT之后该功能会自动开启.

```c
void VAmeter_Mqtt_Send_Data(double volt,double curr)
{
	if(mqtt_status == 0)
		return ;
	double pwr = volt*curr;
	char *data = malloc(150);
	char t[10] = {0};
	strcpy(data,"{\\\"volt\\\":");
	num2char(t,volt,log10(volt)+1,2);
	strcat(data,t);
	memset(t,0,10);
	strcat(data,"\\,\\\"curr\\\":");
	num2char(t,curr,log10(curr)+1,1);
	strcat(data,t);
	memset(t,0,10);
	
	strcat(data,"\\,\\\"pwr\\\":");
	num2char(t,pwr,log10(pwr)+1,1);
	strcat(data,t);
	free(t);
	
	strcat(data,"}");
	MQTT_Publish(data);
	free(data);
}
```

## 3.5 中断、按键、计时器
&ensp;&ensp;整个工程里一共配置了2个基本计时器BTIM，以及1个GPIOA端口的中断，它们各自的功能如下表所示

|中断|功能|
|---|:---:|
|BTIM1|判断按键状态 每10ms进一次中断|
|BTIM2|更新状态栏 每1s进一次中断|
|GPIOA|从休眠中唤醒 仅检测PA10(确认键)引脚|

&ensp;&ensp;此外，还使用通用计时器GTIM3实现屏幕的PWM调光，频率为1250Hz，其初始化函数如下

```c
void GTIM3_PWM_Init(void)
{
	PC15_AFx_GTIM3CH2();		// 复用功能为通用定时器3通道2
	/*********** GTIM3配置 ***********/
	GTIM_InitTypeDef GTIM_InitStruct;  // 通用定时器初始化结构体
	
    __RCC_GTIM3_CLK_ENABLE();		   // 使能通用定时器1时钟
	
    GTIM_InitStruct.Mode = GTIM_MODE_TIME;				 // 定时器模式
    GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;	 // 连续计数模式
    GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV512;    // DCLK = PCLK / 128 = 64MHz/512 = 125KHz
    GTIM_InitStruct.ReloadValue = 100;	 			 	 // 重装载值设置  PWM:1.25KHz
    GTIM_InitStruct.ToggleOutState = DISABLE;			 // 输出翻转功能
    GTIM_TimeBaseInit(CW_GTIM3, &GTIM_InitStruct);		 // 初始化
	
    GTIM_OCInit(CW_GTIM3, GTIM_CHANNEL2, GTIM_OC_OUTPUT_PWM_HIGH); // 配置输出比较通道3为PWM模式
	GTIM_SetCompare2(CW_GTIM3, lcd_brightness);  //设置初始占空比为lcd_brightness/100 = 50/100 = 50%
    GTIM_Cmd(CW_GTIM3, ENABLE);     // 使能定时器
}

```

按键相关的函数在`key.c`源文件中，初始化函数`Key_Init`将各个按键设置为上拉输入模式.

```c
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /*
	PA8 -> UP_KEY	
	PA9->DOWN_KEY 
	PA10->LEFT_KEY
	PA11->RIGHT_KEY
	PA12->CONFIRM_KEY	
	*/
    GPIO_InitStruct.Pins	= LEFT_KEY_PIN|RIGHT_KEY_PIN|UP_KEY_PIN | CONFIRM_KEY_PIN| DOWN_KEY_PIN;
    GPIO_InitStruct.Mode	= GPIO_MODE_INPUT_PULLUP;		//上拉输入
    GPIO_InitStruct.Speed	= GPIO_SPEED_HIGH;			//输出速度高
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);				//初始化
}
```

为了防止按键抖动造成误判，每次定时器BITM1中断(10ms)，都会判断一次按键的状态.`Check_Key_Status`函数用到了状态机.

如果按键为按下状态，就将`key_status`对应的按键下标置为1.然后过10ms再来判断一次，如果仍为按下状态，则是真的按下了而非抖动，将`key_flag`对应的按键置为1. 否则将`key_flag`置为0. 

检测按键长按的方法也是类似，只是多判断几轮，如果按键被按下时间大于130ms，则为长按，将`key_flag`对应按键置为2. 

```c
void Check_Key_Status()
{

	
	for(uint8_t i = 0;i<5;i++)
	{
		if(key_status[i]==0)
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)
			{
				key_status[i] = 1;
			}
		}
		else if(key_status[i] == 1)
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //识别到短按
			{
				key_flag[i] = 1;
				key_status[i] = 2;
			}
			else
			{
				key_status[i] = 0;
			}
		}
		else if(key_status[i] >= 2 && key_status[i]<15)
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //从短按到长按的中间态,按键至少按下了130ms
			{
				key_status[i]++;
			}
			else
			{
				key_status[i]--;
			}
		}
		else if(key_status[i] == 15)
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_RESET)  //识别到长按
			{
				key_flag[i] = 2;
			}
			else
			{
				key_status[i]--;
			}
		}		
		else
		{
			if(GPIO_ReadPin(key_pin_port[i],key_pin[i])==GPIO_Pin_SET)
			{
				key_flag[i]=0;
			}
		}
	}
}
```


# 4 使用手册

# 4.1 PCB焊接
&ensp;&ensp;电路焊接并无难点，最小的封装为0603，还是比较容易焊的.核心器件都采用的是排母插拔式设计，没有什么QFP、QFN芯片的焊接，难度相对较低. 

分享几个在焊接时的小技巧
- 焊接有大面积填充铜的GND焊盘时，适当调高烙铁温度，以防散热过快导致焊锡温度不够，出现虚焊的情况；
- 先焊接各个芯片，如TC4056A、MT3608、LDO等；
- 然后再从小到大、从低到高焊接其他元器件；
- 在焊接开发板两排排母时，可以先把排母插到开发板排针上，再去焊接，这样能保证两排排母间距与开发板的匹配，毕竟开发板排针也是手焊的，难以做到完全垂直.

&ensp;&ensp;使用万用表通断挡测试是否有短路、断路的情况，一切没问题就将立创开发板、屏幕、ESP8266和电池都插入到各自的排母/连接器中.回到电脑上，跟着下面几个小节一步步来做，方可实现多功能物联网的电压电流表.

## 4.2 安装配置Keil & CW32程序下载
### 4.2.1 Keil下载安装
&ensp;&ensp;keil社区版下载与安装在立创开发板的Wiki有详细教程： [立创开发板Wiki安装keil](https://wiki.lckfb.com/zh-hans/dwx-cw32f030c8t6/training/voltammeter-bootcamp/voltammeter.html#_3-2-1-%E5%AE%89%E8%A3%85keil)

### 4.2.2 器件包安装
&ensp;&ensp;器件包安装在立创开发板的Wiki： [立创开发板Wiki器件包安装](https://wiki.lckfb.com/zh-hans/dwx-cw32f030c8t6/training/voltammeter-bootcamp/voltammeter.html#_3-2-2-%E5%99%A8%E4%BB%B6%E5%8C%85%E5%AE%89%E8%A3%85)
 
### 4.2.3 CW32程序下载
（1）下载在**附件**中的`CW32F030C8T6_VAmeter.zip`并解压.

（2）进入文件夹`CW32F030C8T6_VAmeter`->`Project`->`MDK`，使用keil打开`Project.uvprojx`工程文件

（3）配置WIFI.如图18所示，在`main.h`中配置WIFI名称和密码.


![图18 wifi设置.jpg](https://image.lceda.cn/oshwhub/2514a5a3ac6e46c6bbe7ceb8216e4fa2.jpg)
<body>
	<p align="center">图18 修改WIFI名和密码</p>
</body>

（4）配置MQTT.**请在4.5节设置好MQTT服务器之后，再回到本步骤.**

&ensp;&ensp;如图19所示.`username`和`password`是在EMQX服务器上设置好的，具体请见**4.5节**；`ClientID`为自己设置的，我这里为了方便辨识，就设置为esp8266_VAmeter；`服务器地址`为EMQX设备所在的IP地址，而`端口`一般不用动，默认1883；`话题`是要发布消息的话题，这里自己写一个能记住的即可，因为后面在配置Python转发程序时要订阅这个话题.


![图19 mqtt设置.jpg](https://image.lceda.cn/oshwhub/b2ef2db51fb74a59829454603e3c025a.jpg)
<body>
	<p align="center">图19 MQTT连接配置</p>
</body>

（5）Keil下载器配置.如图20所示，我这里使用的是ST-LINK，请根据你的下载器自行选择.


![图20 Keil下载器配置.jpg](https://image.lceda.cn/oshwhub/b5a5b23041f0437d83b1fc581a9a4cf3.jpg)
<body>
	<p align="center">图20 Keil下载器配置</p>
</body>

（6）Keil编译器配置，本项目使用的是 V5.06编译的.安装编译器教程为：https://blog.csdn.net/weixin_44807874/article/details/128627528

&ensp;&ensp;为了方便大家下载，我把该文件从官网转存到网盘里 ： https://www.123pan.com/s/TBfTjv-jdvg.html

（7）点击Keil的Rebuild按钮，编译完成后应为0 Error 0 Warning，如图21所示.


![图21 Keil编译.jpg](https://image.lceda.cn/oshwhub/0e04215120fb481687581d5938afe9b7.jpg)
<body>
	<p align="center">图21 重新编译Rebuild</p>
</body>

（8）连接下载器，点击Download按钮，稍等几秒，即可正常使用.

## 4.3 下载ESP8266的MQTT AT固件
（1）下载并解压**附件**中的`flash_download_tool.zip`和`1112_esp8266-4m.zip`

（2）使用USB转串口模块，依照下表连接串口模块与板载的下载接口

|串口模块|板载接口|
|---|---|
|3.3V|3.3V|
|TXD|RXD|
|RXD|TXD|
|GND|GND|

（3）使用跳线帽，短接下载选择跳线，如图22所示


![图22 下载接线.jpg](https://image.lceda.cn/oshwhub/35e6f1f8d3b74552aeda01b9807589c0.jpg)
<body>
	<p align="center">图22 ESP8266串口下载接线</p>
</body>

（4）将串口模块插入电脑，打开刚解压的`flash_download_tool`文件夹中的`flash_download_tool_3.9.2.exe`，在弹出的窗口中，第一个选择`ESP8266`，第二个选择`develop`，然后点击OK.

&ensp;&ensp;在接下来的界面，按图23所示配置好，COM口根据你电脑识别的选.配置好后点击START.


![图23 flash download.jpg](https://image.lceda.cn/oshwhub/ccd870118c2d4f0bab100fe75acd305c.jpg)
<body>
	<p align="center">图23 固件下载工具配置</p>
</body>

（5）由于CW32程序中默认是关闭ESP8266的，因此，现在需要操作CW32菜单至系统设置--启动ESP8266

**请注意**：保持板载总开关为**关闭状态**，否则可能无法正常下载ESP8266的固件

（6）再看电脑上的flash_download_tool，它就开始下载了，耐心等待进度条走完.

（7）拔掉串口模块接线、跳线帽，操作菜单至WIFI-连接WIFI，查看是否正常连接.也可以在连接后操作菜单至WIFI-网络信息，获取设备IP等信息.

## 4.4 自建InfluxDB数据库
**请注意**：4.4、4.5、4.6节均基于Docker一键安装部署，如果你还未安装Docker，请查看下列教程进行安装：

- Windows安装Docker教程：https://www.runoob.com/docker/windows-docker-install.html
- Ubuntu安装Docker教程：https://www.runoob.com/docker/ubuntu-docker-install.html

&ensp;&ensp;安装完成后，在命令行(windows)或终端(Linux)键入命令 `docker -v`，如果返回`Docker version xx.xx.x,build xxx`字样，则安装成功，可以继续往下看.

（1）拉取InfluxDB镜像

&ensp;&ensp;这里用我自建的一个Docker镜像加速器来做的镜像加速，有可能访问过多了就不可用，如果你有域名的话，可以查看[这篇教程](https://blog.vrxiaojie.top/archives/shi-yong-cloudflare-workerszi-jian-dockerjing-xiang-jia-su) ，利用cloudflare workers自建一个.

如果没有域名也没关系，用我的镜像加速就好了.打开终端，输入以下指令，创建存放数据的目录.

```bash
mkdir config && mkdir data
```
（2）然后运行docker run命令，其中\$PWD是存在终端所在路径的文件夹下.端口映射的话我改成了3086

```bash
sudo docker run -d \
--name=influxdb2.0 \
-p 3086:8086 \
-v "$PWD/data:/var/lib/influxdb2" \
-v "$PWD/config:/etc/influxdb2" \
docker-mirror.vrxiaojie.top/library/influxdb:2.7.6
```

（3）不出意外的话，它会开始自动拉取镜像，没有报错.然后我们打开网页，输入你部署InfluxDB的机器的ip地址:端口，比如我的为192.168.50.111:3086，即可看到欢迎界面(图24).


![图24 influxdb欢迎界面.jpg](https://image.lceda.cn/oshwhub/7455656c720749539d045a0ed39ea678.jpg)
<body>
	<p align="center">图24 InfluxDB欢迎界面</p>
</body>

（4）点击下一步，来到初始化设置，要求我们填写用户名密码、组织名称以及初始Bucket名称，可自己填，也可按我的来(图25).


![图25 influxdb初始化设置.jpg](https://image.lceda.cn/oshwhub/eef39652db2944e7ac3e009bdd61886c.jpg)
<body>
	<p align="center">图25 初始化InfluxDB设置</p>
</body>

（5）搞定！**保存显示出来的API token**，待会会用到.然后点击Quick Start(图26)


![图26 influxdb初始化设置ok.jpg](https://image.lceda.cn/oshwhub/0a8d124179a44d68aae887e56eb4662f.jpg)
<body>
	<p align="center">图26 InfluxDB初始化完成</p>
</body>

至此，InfluxDB数据库就搭建好了.

## 4.5 自建MQTT服务器-EMQX
（1）与搭建数据库类似，另找一个文件夹，输入指令创建EMQX的日志及数据文件所在的文件夹.

```bash
mkdir data && mkdir log
```
（2）然后启动docker容器
```bash
sudo docker run -d --name emqx \
  -p 1883:1883 \
  -p 18083:18083 \
  -v $PWD/data:/opt/emqx/data \
  -v $PWD/log:/opt/emqx/log \
  docker-mirror.vrxiaojie.top/library/emqx:5.7.1
```

（3）打开浏览器，进入你部署EMQX的 ip地址:18083，比如我的为192.168.50.111:18083，进到登录界面，EMQX的默认用户名`admin`密码`public`，登录后修改密码，进入主界面.

（4）创建客户端认证，见图27.客户端认证-->创建-->"Password_Based" 下一步--> "内置数据库" 下一步-->"username" "sha256" "suffix" -->创建


![图27 EMQX 创建用户1.jpg](https://image.lceda.cn/oshwhub/b74bd7330c70420b88e6419f5b358d99.jpg)
<body>
	<p align="center">图27 EMQX创建客户端认证</p>
</body>

（5）创建成功，点击用户管理(图28)-->“+”号-->输入用户名，我写`esp8266`，密码123456-->创建 (图29).

&ensp;&ensp;同理，创建个用户名叫`python`的，以便下一节使用.


![图28 EMQX 创建用户2.jpg](https://image.lceda.cn/oshwhub/60eb8d8e7c994643aa16fb976de5e812.jpg)
<body>
	<p align="center">图28 创建用户</p>
</body>


![图29 EMQX 创建用户3.jpg](https://image.lceda.cn/oshwhub/19f40e7738fd4fc5b0d19e0a40f6fdc3.jpg)
<body>
	<p align="center">图29 设置客户端用户名和密码</p>
</body>

（6）最后，回到**4.2.3节的步骤（4）**，在宏定义中把相关参数填入，再下载程序到CW32.

## 4.6使用python程序接收并转发数据至数据库
（1）下载**附件**中的`python转发程序.zip`并解压，进入该目录，打开终端运行，安装依赖库.
```bash
pip install -r requirements.txt
```
（2）完成之后，使用任意编辑器打开`emqx_receive_VA_meter.py`，修改我标注`#修改此行`的那些参数.如图30所示.改完记得保存.

![图30 修改python程序.jpg](https://image.lceda.cn/oshwhub/a2a9f31c45f447df8ff7ed775178fcc5.jpg)
<body>
	<p align="center">图30 python程序参数设置</p>
</body>

（3）运行py程序.在当前目录打开终端，输入
```bash
python emqx_receive_VA_meter.py
```

如果出现`Connected to MQTT Broker!` 说明成功连接到EMQX服务器了，把该程序放在后台即可.

（4）打开电压电流表电源开关，依次连接WIFI、MQTT，并进入测量电压电流功能.此时再来看终端上，会显示出电压、电流、功率的数值，如图31所示


![图31 python程序接收到MQTT数据并解析.jpg](https://image.lceda.cn/oshwhub/74de598dc10547f1a1c1e324352e5322.jpg)
<body>
	<p align="center">图31 程序解析到的数据</p>
</body>

（5）进入InfluxDB后台管理界面，按图32所示操作，可查到刚刚发送上来的数据.


![图32 在db看数据.jpg](https://image.lceda.cn/oshwhub/0263569b810246ca88e18f760264506e.jpg)
<body>
	<p align="center">图32 InfluxDB数据查询</p>
</body>

## 4.7 Grafana数据监控仪表盘
（1）使用Docker安装grafana.先创建数据文件夹
```bash
mkdir grafana
```
（2）然后docker run
```bash
sudo docker run -d --name=grafana \
-p 3000:3000 \
-v "$PWD/grafana:/var/lib/grafana" \
-e GF_DASHBOARDS_MIN_REFRESH_INTERVAL=100ms \
docker-mirror.vrxiaojie.top/grafana/grafana:11.0.0
```

（3）打开浏览器，进入你部署grafana的 ip地址:3000，比如我的为192.168.50.111:3000，进到登录界面.默认用户名和密码都是`admin`

（4）创建数据源.如图33所示，点击左侧栏的Data Sources，点击Add New..


![图33 gra加数据源.jpg](https://image.lceda.cn/oshwhub/904a8d75f23446a9b02a095e92028f1c.jpg)
<body>
	<p align="center">图33 Grafana添加数据源</p>
</body>

选择InfluxDB，然后在接下来的页面按图34、35填写.


![图34 gra加数据源2.jpg](https://image.lceda.cn/oshwhub/ca28d2a5ac6a40e0909221abae144a27.jpg)
<body>
	<p align="center">图34 数据源配置</p>
</body>


![图35 gra加数据源3.jpg](https://image.lceda.cn/oshwhub/dbcf3b6323e44532936eb24524546d74.jpg)
<body>
	<p align="center">图35 数据源配置</p>
</body>
（5）点击主页右上角的"+"号，选择New Dashboard，点击Add visualization，选择刚才创建的数据源.这里我就加一个电压曲线图作为演示，其他几项都是类似的.

&ensp;&ensp;回到InfluxDB后台，在刚刚我们查询数据的地方，按图36的操作来，然后把图37所示的区域代码给复制下来，粘贴到Grafana中(图38). 一些样式可以在右侧栏调节完成后点击Apply 和 Save.


![图36 db查询 脚本编辑.jpg](https://image.lceda.cn/oshwhub/b83995b9595040d1865489e72ab52b37.jpg)
<body>
	<p align="center">图36 数据查询操作</p>
</body>


![图37 db查询 脚本复制.jpg](https://image.lceda.cn/oshwhub/1cf0e44537524121b111d946e2b388ac.jpg)
<body>
	<p align="center">图37 获取查询语句</p>
</body>


![图38 粘贴查询语句.jpg](https://image.lceda.cn/oshwhub/e36cda78505241bf91896c444a3416b2.jpg)
<body>
	<p align="center">图38 粘贴查询语句</p>
</body>
（6）把电流和功率的也用上面类似操作做好.最终效果见图39a与39b.


![图39 最终效果.jpg](https://image.lceda.cn/oshwhub/6abb6a3f1b484f9db7f2f27288a81f27.jpg)
<body>
	<p align="center">图39a Grafana仪表盘 PC</p>
</body>


![图39b 最终效果phone.jpg](https://image.lceda.cn/oshwhub/b29fa97cf04241e2b193bf2d3be08541.jpg)

<body>
	<p align="center">图39b Grafana仪表盘 手机端</p>
</body>

# 5 外壳组装
面板下单注意事项：
- 面板类型：亚克力面板
- 面板款数：多款自主拼版
- 面板尺寸：192.78mm 95mm
- 拼版套数：1套
- 不需要确认生产稿
- 透明亚克力，厚度1.5mm或2mm
- 打印方式：正面
- 遮光强度：常规
- 不需要背胶

拿到面板后，要用螺丝等紧固件装配，需要准备的材料如下表所示
|零件|数量|
|:---:|:---:|
|M2*5 扁平头螺丝|8|
|M2*8双通六角铜柱|4|
|M2*8+3单通六角铜柱|8|
|M2螺丝帽|8|

各个零件装配位置如图40-42所示
![图40 顶.jpg](https://image.lceda.cn/oshwhub/d479d04b40a8470d9a6bc38b0b86c606.jpg)
<body>
	<p align="center">图40 顶部视图</p>
</body>


![图41 侧面.jpg](https://image.lceda.cn/oshwhub/01141851f760449292bf8e0ca69ebecd.jpg)

<body>
	<p align="center">图41 侧面视图</p>
</body>


![图42 底面.jpg](https://image.lceda.cn/oshwhub/29b90ef28b744058bc9bb99b0e7517e5.jpg)

<body>
	<p align="center">图42 底部视图</p>
</body>

# 6 链接
本项目工程开源在开源广场：https://oshwhub.com/vrxiaojie/voltage-and-ammeter-based-on-lic

软件代码开源于Github：https://github.com/wjj0227/CW32F030C8T6_VAmeter

软件API文档：https://wjj0227.github.io/CW32F030C8T6_VAmeter/files.html

B站：https://space.bilibili.com/11526854

个人Blog：https://blog.vrxiaojie.top/

# 7 开源协议
GPL 3.0

# 8 演示视频
高清版见B站： [多功能物联网电压电流表 基于立创地文星开发板CW32 ESP8266](https://www.bilibili.com/video/BV1WhYxeWEEA)

</span>