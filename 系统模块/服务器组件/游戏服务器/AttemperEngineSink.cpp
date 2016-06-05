#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////
//时间标识

#define IDI_LOAD_ANDROID_USER		(IDI_MAIN_MODULE_START+1)			//机器信息
#define IDI_REPORT_SERVER_INFO		(IDI_MAIN_MODULE_START+2)			//房间信息
#define IDI_CONNECT_CORRESPOND		(IDI_MAIN_MODULE_START+3)			//连接时间
#define IDI_GAME_SERVICE_PULSE		(IDI_MAIN_MODULE_START+4)			//服务脉冲
#define IDI_DISTRIBUTE_ANDROID		(IDI_MAIN_MODULE_START+5)			//分配机器
#define IDI_DBCORRESPOND_NOTIFY		(IDI_MAIN_MODULE_START+6)			//缓存通知
#define IDI_LOAD_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+7)			//系统消息
#define IDI_SEND_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+8)			//系统消息
#define IDI_LOAD_SENSITIVE_WORD		(IDI_MAIN_MODULE_START+9)			//加载敏感词
#define IDI_DISTRIBUTE_USER		    (IDI_MAIN_MODULE_START+10)			//分配用户
#define IDI_CHECK_STOCK				(IDI_MAIN_MODULE_START+11)			//库存时间读取，写入
#define IDI_GET_USER_LUCKY			(IDI_MAIN_MODULE_START+12)
#define IDI_DISTRIBUTE_PRECETABLE   (IDI_MAIN_MODULE_START+13)			//重新分配优先桌子
#define IDI_CLOWN_FAKE_MSG			(IDI_MAIN_MODULE_START+14)			//小丑假中奖消息
#define IDI_READ_CONFIG				(IDI_MAIN_MODULE_START+15)			//定时读取配置
#define IDI_CLOWN_BIG_REWARD		(IDI_MAIN_MODULE_START+16)			//广播巨奖
//#define IDI_READ_MATCH_CONFIG		(IDI_MAIN_MODULE_START+17)			//读取比赛配置
//#define IDI_TIMER_CHECK_INTERVAL	(IDI_MAIN_MODULE_START+18)			//比赛时间检测
//#define IDI_MATCH_BEGIN_REMINDER	(IDI_MAIN_MODULE_START+19)			//比赛开始倒计时

#define IDI_MATCH_SIGN_UP			(IDI_MAIN_MODULE_START+20)			//报名时间
#define IDI_MATCH_TIME				(IDI_MAIN_MODULE_START+21)			//比赛时间是否开始
#define IDI_MATCH_GET_USERSCORE		(IDI_MAIN_MODULE_START+22)			//定时获取用户分数

//////////////////////////////////////////////////////////////////////////////////
//时间定义 秒

#define TIME_LOAD_ANDROID_USER		600L								//加载机器
#define TIME_DISTRIBUTE_ANDROID		8L									//分配用户
#define TIME_REPORT_SERVER_INFO		30L									//上报时间
#define TIME_DBCORRESPOND_NOTIFY	3L									//缓存通知时间
#define TIME_LOAD_SYSTEM_MESSAGE	3600L								//系统消息时间
#define TIME_SEND_SYSTEM_MESSAGE	10L								    //系统消息时间
#define TIME_LOAD_SENSITIVE_WORD	5L									//加载敏感词时间
#define TIMER_CHECK_STOCK_OP		20000L								//库存时间读取，写入
#define TIMER_GET_USER_LUCKY		20000L								//用户幸运值时间
#define TIME_DISTRIBUTE_PRETABLE	20L									//加载敏感词时间
#define TIME_SEND_CLOWN_FAKEMSG		600000L								//发送小丑加信息的时间
#define TIME_READ_CONFIG			60000L								//定时读取数据
#define TIME_MATCH_SIGN_UP			5000L								//比赛报名时间
#define TIME_MATCH_DURATION			5000L								//比赛持续时间
#define TIME_MATCH_GET_USERSCORE_INTERVAL	5000L						//获取分数时间间隔
//////////////////////////////////////////////////////////////////////////////////

char *  g_pszFakeName[] = {"速战速决","光辉岁月","因帅被判无妻","齐逼小短裙","闲情逸致","不懂爱的人","笑饮砒霜","跟往事干杯","勇者之王","胸有大痣","久了就旧了","迷失在人海","邪恶老烟枪","第二炮半价","老衲下山采花","天然野生帅哥","一岁就很帅","奔腾草泥马","真心喂过狗","酷派哥","杰叭与不叭都一样","ushdhdebgd","bvddfghhjj","augvss","dghjjhg","142588","Hggtvh","第一副","减肥茶","hjij66663","dfghhbbbn","cvgrdd","vuhghjj","0459zd","无雨","咯咯大笑不止","yvdexm","suggshsh","ydrrdrrr","bxbdveb","okjkohk","jpz011","jpz009","杰爱果子","6647568","duliang","Hsshshsjs","zg747606","Csydhsshshs","估计老婆","ghjeeff","李娜999","369077","nnjxjje","苦命卡纳克纳","轰轰烈烈","水泥七彩虹","sdjp18","彩金小王子","9467852","Ywijhmh","最后十块","hdghxvgdv","dududu","dtghhjjn","Zhengjing6","心灵捕手","预告繁花似锦","shouldhave","铁路局模型","沓沓","ffuvuc","nimala","知足常乐","小马8287336","勇猛无比","ai5215","wqasewa","lw7777lw","Qsadsrff","学渣代言人","来地球逛逛","保安大队长","花落知多少","顾我心安","浅黛微妆","脸红气息","d调的华丽","恭喜发财","不是迩的错","请叫我大叔","活在当下丶BSW","大尾巴狼","92萌叔叔","y1个人也能活","潇洒的微笑","世上没有如果","铭记这段殇","暧昧控","格式化丶孤单","_少年已不年少","淡忘","领悟","留点小胡须","戒浮戒躁","adsferf","男儿当自强","绝世淫鱼","师太借个吻","浅唱呐回忆","半岛酒店","喵了个HelloKitty","跟寂寞和好","飞龙在天","闹钟你别闹","人心太拥挤","慕后煮屎者","机器人的爸爸","追着幸福跑","完美舞步","薰衣草的眼泪","说谎我会笑","海绵宝宝","苏格拉没有底","山炮中的极品","刺痛硪的心","留不住的都滚","冷酷的我忐忑","狠狠平凡","斯人独憔悴","过客匆匆","惜笑红颜罪","曾相守勿忘","社会狠残酷","墨小颜","腻了就滚吧","墨墨","倚楼听雨声","浪里个浪","一岁就很帅","囚心176","不气补脑","暮色微凉","指尖落樱舞^_^","举个栗子","人来狗往","人美B遭罪","粗人一枚","凉水刺喉","梦要自己扛","你别皱眉","孤人眠","猪小哼","蔷薇仙子","开心就好","向地狱狂奔","不期而遇","开心就好","不期而遇","再笑乜是伤","手起刀落人抬走","半明媚半忧伤","[学渣代言人]","资深宅男","爱要怎么喊停","开心就好","风情乱种","拥久则安","惊鸿过影","月下唱离歌","年少猖狂必成皇","孤身撑起一片天","逗比卖萌无所不能","你不配情深","正在输入中","鱼筷到碗里来","不高丶不富丶不帅","我在北京地铁战","深海没有雨季","低调糕富帅","梅西0012","请你次辣条","没刘海照样拽","带我装逼带我飞","隔壁王学长","装逼不适合你","有钱就是任性","洞房不败","嘟嘟糖果","蜡笔小旧","骑白猪的王子","别低贱了自己","河南鱼神灬","想赢别喊停","全国帅比代表","柠檬草味道","海倒过来是天","正太大叔","阳光气质男","愿得一人心","爱情别虚伪","钓鱼佬888","很爷们","小草先森","我爱武则天","凄凉夜太美","花落丶莫相离","逍遥捕鱼","马六甲捕鱼","地狱使者","狂战天下","为自己而活","花心本人戒了","妞给根烟抽呗","菊花少年","我是你的男神","夜凄凉","童话皆是假","天比不过海的蓝","冬约夏至","人鬼殊途","闲来雅叙","纪委叫我混社会","往事如烟","终止放荡","拜拜爱过","到此为止","慢慢习惯自然","一首简单的歌","霸占女厕所","依然在起","逝水无痕","怡红院花魁","低调沉默者","冰动尘心","沸水中的雪","爱情式男人","狠狠爱","金鱼的眼泪","小妖","企鹅先生","鳄鱼喵喵叫","忧伤嘉年华","隗语琴","一朵寂寞的花","思念是一种病","心不动则不痛","90后丨帅男","爱到了心痛","夜美的凄凉","现实太现实","心如此悲伤","迷惘的沿途","注定一个人","情话大师","笑敬过往","茶凉缘尽","蕾丝女Humane","扯淡的微笑","谎言谁都会说","等不到的你","让寂寞远行","单身天涯","_浅蓝色De夏","水清云淡","会很幸福","醉卧美人膝","踏雪寻梅","竺建柏","竹影清风","残荷听雨","天镜云生","烟锁寒楼","春眠不觉晓","蝶梦无边","清水幽萍","漾漾涟漪","漫游者","幽逝无痕醉","上善若水","康鸿博","王菲追我半条街","丛宇文","风袖薇香","故乡的云","柏浩大","素颜倾城几度妖娆","寂寞累了","页落","游吟诗人","夜的不平凡","遗憾的那段情","优柔寡断","爱生活爱拉登","逆光少年","慕容听兰","哥特式祸水","男人也会不坚强","正儿八经小骚男","小男人大智慧","安静的美男子","这么近那么远","岁月乱了浮华","久居孤岛无人问","从不低调","藐视大地","微微一笑醉红尘","有些爱划不来","夏天那抹蓝","小新只属于蜡笔","大爱斯文","情飞扬","空心少年","心在寂寞","哆啦C梦","枯萎罂粟花","友谊不毕业","梦入他心","偷心大盗","蛋卷","再回首已陌路","尾戒的爱","亚里士多缺德","造化弄人","颜小熙","未完待续","皇上是喜脉","永不毕业的友谊","你是我的信仰","对不起查无此人","兜兜里有糖","100纯二货","捕鱼达人就是俺","裸泳的鱼","善解人衣","帅的想跳楼","高冷de逗逼","加载中99","蒙牛没我牛","正在缓冲","Royal大懒猫","小土逗","萌妞优酸乳","咿呀咿呀哟","骑蜗牛撵大象","爱赢才会拼","转让半包辣条","连起wifi聊死你","丑的无可挑剔","笑傲任我行","摸扎算命","坎坎坷坷乐呵呵","姓坚名强","男人站直别趴下","心向阳无谓伤","我为自己代言","破茧成蝶","青春不言败","哥4拽","说爱太烫嘴","辣条协会会长i","暧昧乱人心","泡八喝九说十话","向着幸福钱进","无聊也不聊","法克鱿","梅川酷紫","勿忘心安","孤毒","油炸皮卡丘","巷尾暖阳白衬衫","转身说再见","砸锅卖铁来上网","残缺的温柔","装逼界扛把子","一颗小逗比","冷眸笑红尘","黑名单欢迎你","诗酒趁年少","把酒临风","米阳光","ugugugw","快捷键快快","njf566","蓄势待发","jedndbj","liangshanbo","tugugug","hsjsjhsj","gjefduf","3ed4dx","wc80077","hvcdsf2fhjjj","eudjejm","dounimei","想逆袭一下","gbthghgihhh","sdjp16","ddtyyuff","junjun520","dhxydgennx","打鱼铺子","vyvyvubvb","瑞普汽车","udndjdjfidj","德国等发达国家","柳岩","地狱13","dhfjshs","小刚王者","123456沫沫","hduderh","HRsitser","jxx41115","ytfghrd","nishikou44","sgxxhh","qtjmxj","hdhd222","hcijfnkddd","eedddsjjejsee","qianerdenger","wwhuhddhh","迷津桥头","xfghrdcjkkm","liuyangming8","zo747nii","uddjjd2222","幽幽释然","他墨迹咯哦","青梅骑着竹马","喵^^喵","大懒猫","欢迎骚扰","一颗卤蛋_","喂说你呢","迩闹够了没有","禽小兽^^","黯然销魂掌","上帝是个妞","骑母猪闯红灯","我是二逼怕谁","弹走鱼尾纹","情水深王八多","海誓山盟屁丶","来盘烤翅","屎里逃生","32号我娶你","兔比南菠丸","且行且珍惜","骑母猪闯天下","格子控","爱如困兽","无名指忧伤","浪哩个浪","不一样的烟火","少年先疯队","爱我者昌","戒爱","记忆斑驳了","心岛未晴","我是路人甲","其实我不傻","huxian888","xsssssj","zl1382","好运伴我赢","卢造么123","打不死卸载","iuy9688","abcdbb","wz199611","夜游鱼神","dq2563","ssseezzz","zo8787","jzk130708","tu908553","gyffd3555","xgfkfhg","hnhfguuu7","mnbhjkl123","bnnhgtffff","123djj","huxian777","accdaaaa","严建设","喵喵喵喵喵","wwwewweq","dq13468","123djm","023123vvkfjrt","x814420053","zzzjjj88","Gujnnj","kkjnxkbbji","uvf67i8","迷你彬123","Chu兔吐太冷了","华而不实1","rgijgg","jsnsnsnxs","孩儿们给爷倒茶","甜美即可","rddgg448","捕鱼梦琪","vbhgyunjik","jjdjdde","fghjkkhj","bhdhvsge","hdbxbdjdn","vgy123580","nimeim","大大的炮123","Zhangge","e888888e","kkkeerp","ffgtgtgw","nsnsnsnd","仅此而已yyy","hgfu8765","没事找事i","大战三百年","123z4g5m","r561826574","zhaihao","cccxxxz","fxfgg1478","nsnwwww","wc88imdii","jxhgdgd","xcv11111","ygffvh","jw77178ej","aiaiaiai","tuvf668","bdu6wh8i","你好点","jjjjnjnnh","ftygfhuh","ycyfhchvj","jspeibipo1","hbdbjdhsh","我行我素464","gddyjh","鱼雷炸了","yangyan","fig8f8f8","黄飞38","4ch6bj","eraaatj","gsggshhsh","我就不是你了","jssnsnseee","33441122","srffgvhhj","看看咯磨破","优雅气质","zsyzjl123","老鼠也","ghjcgcg","xbhdhdjzj","rgyihf","飞龙008","dtfhvhh","4r5668","jkajtdnw","rfguiiooll","cyydgj","你咯logo","说今天恢复","心情小跟班","几楼出来聊聊","6869583","12rsftgg","nsnsnsss","火柴0317","2271343231","海滩度假","fdsuhggji","黑天使","技术监督局的","姓柳名下惠","xiaowan","鱼雷8","dingggnajhg","jhffddhhj","bdhchhfyd","小孩纸的J","gucgju","d666666d","呒所","好方法好几年","jyfddt","无爱过","小宝宝萌","gugugyyj","努力了就行了吧","gghhjnv","Dingsidang","有哦咯一样","w24j12x11","gdjjjrbfd","dfvyyhj","引吭高歌更好","jdhgejdvd","teghehdh","787yuyy7t6y7","Xnxidsn","15515515wu","a895558","pjantgan","jsjsjssjshsh","tyu677","jhdhdhfn","12315q","我我我你说11","cm摸咯血po","xvxgdgd","虞姬姐","dmwgua","vy316419js","李阿丽","ttd3467","美女配色","king马克大卫","网撒千条鱼","dhhvdgh","章子怡12323","jdhfhdjhdve","聊咯合谋ni","鱼雷","fyhbhh","dssryhg","lkkijo","给我下大鱼","lovemm","ftgmjg","dgdgueuege","中华六","大家觉得仿佛好","qingang1","ysgeywgjsh","d12332","a8957480","gfh654","vjghcf","jdhdbbdbddbn","wzh530","qwevbjk123","韩国发货","bae111鱼雷","huzijjdajjda","Xiao鱼","dmtptdt","Bxjsne","也许会笑呵呵","hcheue369","自觉培养","xl461126464","gdjhdtuf","yiyuhen","asz898","浙江杭州","w7777lw","w6666lw","xtrxtrysss","ss1314520","jjuuiju","同乐山西","杨芙玲","yyyyyyying","etwyug","只是匿名","来咯管15745","fhfbfcbdnd","548064","老朋友a","包aaaaaa","gggfdee","111111w","wyx333","jxl1314","12377y","功夫DJ","jhcrty","rrttyyuuii","梁子998","vcdtgb","聪少666","罗志","dtugdrj","ghssef","莉香姐","nhfdry","n00544","hbvfdewsxz","hsgech","ghggggb","jsnsmddd","hgtyhg6","第二炮免费","裹脚布","11111f","seezddeewe","dongzi5","芬芳岁月","刚哥无敌版本","a15fgcfff","句话不","hhhyivkv","gghgggh","李即将","yhgghl","qwsddff","dongzi4","252525nn","yyegggdggh","frgji5536","a2863115082","usjejesww","tszxhs","w6719625","zxcvbnmlinb","wrjfiydi","a65001234","外边呢11","祖玛教主110","a15070041","dongzi3","fmfjhhjyfj","老狼来也","专家打鱼","uoa5866999","大鱼快下来","打算鱼","jhjhuuhjj","778899oo","aaz1234","滑冰场","djjjjjkk","hahaasre","赖忠祥","u好v健康","尽管非官方","puj2888856","Hdjfjfjfjf","139968146","agily16","777777f","xx09616","yesyes777","sssszxd","a15070041","hai1234567","ghjhvbn","q6445320","hehe2520m","dongzi1","lililiuliu","我就不相知11","赵云子龙","hhhhh222222","拖拉LadyGaga","FZMCZD","tchvds","才几点继续","59383566kk","mo1363825","lukais62868","啊哈哈哈哈哈","490677","小马如何","孤胆英雄","踩踩踩踩c","dfghhjjv","开车快点快点","juslljz","记得查看宝宝","xxxdfrs","wzh158","tyffggtdsfg","agily15","djdkww","巅峰丶捕鱼","wangyui96","huij6366","a15070041kf","jhdhfmf","jdshh34","yuan137094","很关键核心","huozhi","碰到你的活","ghgffgniu6fg","ngfmjuggr","ghjjgvh","杨桃","奖金里咯了","dghffd","a650se","cdtdff","520336","来咯感觉","666tsfwftstw","今晚打老虎ba","kkriben","cvhhhyhh","hhcdryjj","润肺你猜","下降到8","weqasdewa","ddfhvffg","xdfvvcf","Hejhzhdd","555555f","julijuzu","123456wasn","agily14","huzijjdajj","axuda6500","休息休息xx","jdhhhrhj","888888x","输必鱼扑","江湖","我就去哪了11","ajdjjdjd12","许许","ijuyttti","水泥六月","mdkjdhffj","大好人","610289","fgt123","还有他人的","zk747606","jlh814","诸葛亮jiqiren","天天输","xuziyuan666","我想你啦手机","小高123","横刀向天笑为","opl87072","987丽","liuyang660","243320242","yyysss14","gplwun","jqx123","guigui12","woshishuiq","hzhhbshsh","529077","binbinbin","wccc8207","喝口呼吸一下","hdbfhgfg","shijie789","890890a9","wywy51","mg528885","uhggff","dkeem433","fresrew","水泥五百","fhjjfg","snbxhxjzjjsks","小子们有点数","Ehejhsn","yoyo52000","酒逢知己X","wcc88207","hgffhjf","维生素D","小伟捕鱼","A497396","jpgmwjat","7845124","6tuyffhjj","fghjcfhvcgb","jt12356","性感kk","fhfufhfy","jdbh678","qylpcb","ghjkkkkggjk","hjtpgaujg","woshini","啥时候是个头","xfso222","kk284628","iiii646464","zu74760","m96n649","wc82078","bhjycvg","我落伍图秀秀","qtmkwn","SisterHeRui","xgfhff","xk19890526","xbjdbjdjj","我是不是我","mwtppj","流浪者","y52669","sdjp169","ak47ak","土豪哥哥","你给伤害","ye12356","lilipen","s4gf123","自营每平米","cbgdjc","土豪在此","1987118al","yu达人yu","343591037b","dbbxdee733","和天空","尼亚","宝贝乖不乖","iijoll","cy2234","爆机的节奏i","薛刚超","水泥四月","1q1q2w3e","喂等一下","bb142536","tgtddhr","grhjuhty","23748392","gdfigc","jyujenwt","yangjjj111","a15070041","3wangli","即可好看好","大家的办法","lzc8889","456712aq","wmwmpk","水泥三","jiagang002","东京都7","Hw丶无奈","dd14253","A15132","wanglli2","dcjkfesxvh","QsDsddsf","RMB8000","bhft5678","要ni快点完","wang7409","mwpvajt","Yufyvyyvu","卧铺","buyummm","a1507004174","bffhihy","啊上帝能否","260766","雨蒙蒙杀死","kughjyg","法国恢复","namjdjekdmd","我爱你1314","二十八年","都很方便不","水泥二","hdgdgr","ly510125","a91271","978568","zjsnjzjz","rir9ed","发达的规范","天天向上","556680009","俺的金额对","sdfg168","890223nijuan","qwsjjs","飞龙007","a15070804","eeieokfd","西南地区","a2010222","d等待的那么久","Kl7789jsj","xxdfghj","q866052","z7203313","1wangli","hua花2222","long66635","ghjjjjjjjf","大江大海","345678ii","ghzjs556889","cc806500","jhhgbnjjj","豆腐花","当年的3","yffujg","ly520061","qa866052098","ygfgj999","aa0708043","awangli","水泥一","Fe0909","ppplllaa","小黑脸","dmtpwjt","妹妹醉了","ujjhgcvh","sfvezfd","哦婆婆哦楼哦","jfkfkfmff","poiioooo","第一份互粉互","ptmwmahx","在大多数的","ydhh123","shgdyew"};


//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//绑定数据
	m_pNormalParameter=NULL;
	m_pAndroidParameter=NULL;

	//状态变量
	m_pInitParameter=NULL;
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIAttemperEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pIGameServiceManager=NULL;
	m_WaitDistributeList.RemoveAll();

	//数据引擎
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;
	m_pIDBCorrespondManager=NULL;

	//配置数据
	ZeroMemory(&m_DataConfigColumn,sizeof(m_DataConfigColumn));
	ZeroMemory(&m_DataConfigProperty,sizeof(m_DataConfigProperty));

	//比赛变量
	m_pIGameMatchServiceManager=NULL;

	memset(&m_TotalStock, 0, sizeof(m_TotalStock));
	m_TotalStock.mRoomID = -1;
	m_TotalStock.mTableID = -1;
	m_TotalStock.mType = eStockType_Player;

	memset(&m_RobotStock, 0, sizeof(m_RobotStock));
	m_RobotStock.mRoomID = -1;
	m_RobotStock.mTableID = -1;
	m_RobotStock.mType = eStockType_Robot;

	memset(&m_LotteryInfo, 0, sizeof(m_LotteryInfo));

	memset(&m_MobileMoney, 0, sizeof(m_MobileMoney));

	srand((unsigned int)(time(NULL)));
	m_RandPartNum=rand()%4+1;

	AndriodSitTableAssign();

	m_wFakeAnnounceTimeInterval = 60000;
	m_lBigReward = 2166650458;
	m_lBigRewardAddBaseNum = 100;
	m_lBigRewardAddRandNum = 900;
	m_lBigRewardBaseNum = 2166650458;
	m_lBigRewardMaxNum = 8000000000;
	m_MatchConfigItemList.clear();

	// 比赛变量
	m_bMatchSignUpMark = false;
	m_bIsMatchTime = false;
	m_nCurrentMatchID = 0;
	m_wUserCntInGroup = 8;
	printf("constructor m_nCurrentMatchID\n");

	m_vMatchUserInfo.resize(100);

	return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
	//删除数据
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//删除桌子
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//清理数据
	m_KickUserItemMap.RemoveAll();
	ClearSystemMessageData();
	m_WaitDistributeList.RemoveAll();

	for (std::list<tagUserLucky *>::iterator iter=m_UserLucky.begin(); iter!=m_UserLucky.end(); iter++)
	{
		delete *iter;
	}
	m_UserLucky.clear();

	m_MatchConfigItemList.clear();

	return;
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IMainServiceFrame,Guid,dwQueryVer);
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定信息
	m_pAndroidParameter=new tagBindParameter[MAX_ANDROID];
	ZeroMemory(m_pAndroidParameter,sizeof(tagBindParameter)*MAX_ANDROID);

	//绑定信息
	m_pNormalParameter=new tagBindParameter[m_pGameServiceOption->wMaxPlayer];
	ZeroMemory(m_pNormalParameter,sizeof(tagBindParameter)*m_pGameServiceOption->wMaxPlayer);

	//配置机器
	if (InitAndroidUser()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//配置桌子
	if (InitTableFrameArray()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置接口
	if (m_ServerUserManager.SetServerUserItemSink(QUERY_ME_INTERFACE(IServerUserItemSink))==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//启动机器
	if (m_AndroidUserManager.StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置时间
	m_pITimerEngine->SetTimer(IDI_GAME_SERVICE_PULSE,1000L,TIMES_INFINITY,NULL);

#ifdef _DEBUG
	m_pITimerEngine->SetTimer(IDI_LOAD_ANDROID_USER,10000L,TIMES_INFINITY,NULL);
#else
	m_pITimerEngine->SetTimer(IDI_LOAD_ANDROID_USER,TIME_LOAD_ANDROID_USER*1000L,TIMES_INFINITY,NULL);
#endif

	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_ANDROID,TIME_DISTRIBUTE_ANDROID*1000L,TIMES_INFINITY,NULL);

	m_pITimerEngine->SetTimer(IDI_DBCORRESPOND_NOTIFY,TIME_DBCORRESPOND_NOTIFY*1000L,TIMES_INFINITY,NULL);

#ifdef _DEBUG
	m_pITimerEngine->SetTimer(IDI_LOAD_SYSTEM_MESSAGE,15*1000L,TIMES_INFINITY,NULL);
	m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_MESSAGE,5*1000L,TIMES_INFINITY,NULL);
#else
	m_pITimerEngine->SetTimer(IDI_LOAD_SYSTEM_MESSAGE,TIME_LOAD_SYSTEM_MESSAGE*1000L,TIMES_INFINITY,NULL);
	m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_MESSAGE,TIME_SEND_SYSTEM_MESSAGE*1000L,TIMES_INFINITY,NULL);
#endif

	//延时加载敏感词
	m_pITimerEngine->SetTimer(IDI_LOAD_SENSITIVE_WORD,TIME_LOAD_SENSITIVE_WORD*1000L,TIMES_INFINITY,NULL);

	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_PRECETABLE,TIME_DISTRIBUTE_PRETABLE*1000L,TIMES_INFINITY,NULL);
	
	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_USER,10*1000,TIMES_INFINITY,NULL);

	m_pITimerEngine->SetTimer(IDI_CHECK_STOCK,TIMER_CHECK_STOCK_OP,(DWORD)(-1),NULL);
	m_pITimerEngine->SetTimer(IDI_GET_USER_LUCKY,TIMER_GET_USER_LUCKY,(DWORD)(-1),NULL);
	//2008是超级海盗游戏类型
	if (m_pGameServiceOption->wKindID == 2008)
	{
		m_pITimerEngine->SetTimer(IDI_CLOWN_FAKE_MSG,m_wFakeAnnounceTimeInterval,(DWORD)(-1),NULL);
		m_pITimerEngine->SetTimer(IDI_READ_CONFIG,TIME_READ_CONFIG,(DWORD)(-1),NULL);
		m_pITimerEngine->SetTimer(IDI_CLOWN_BIG_REWARD,5000,(DWORD)(-1),NULL);
	}
	//2001是捕鱼类游戏
	//if (m_pGameServiceOption->wKindID == 2001)
	//{
	//	m_pITimerEngine->SetTimer(IDI_READ_MATCH_CONFIG,60000,(DWORD)(-1),NULL);
	//	m_pITimerEngine->SetTimer(IDI_TIMER_CHECK_INTERVAL,60000,(DWORD)(-1),NULL);
	//}

	//比赛时间
	m_pITimerEngine->SetTimer(IDI_MATCH_SIGN_UP, TIME_MATCH_SIGN_UP, (DWORD)(-1),NULL);
	//m_pITimerEngine->SetTimer(IDI_MATCH_TIME, TIME_MATCH_DURATION, (DWORD)(-1), NULL);

	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//状态变量
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//配置信息
	m_pInitParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;

	//数据引擎
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;

	//绑定数据
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//删除桌子
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//删除用户
	m_TableFrameArray.RemoveAll();
	m_ServerUserManager.DeleteUserItem();
	m_ServerListManager.ResetServerList();
	m_WaitDistributeList.RemoveAll();
	
	//停止服务
	m_AndroidUserManager.ConcludeService();

	//停止比赛
	if(m_pIGameMatchServiceManager!=NULL)
		m_pIGameMatchServiceManager->StopService();

	//清除消息数据
	ClearSystemMessageData();

	//复位关键字
	m_WordsFilter.ResetSensitiveWordArray();

	return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_CONNECT_CORRESPOND:		//连接协调
		{
			//发起连接
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//构造提示
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//提示消息
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case CT_LOAD_SERVICE_CONFIG:	//加载配置
		{
			//加载配置
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_PARAMETER,0L,NULL,0L);

			//加载列表
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_GAME_COLUMN,0L,NULL,0L);

			//加载机器
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_ANDROID_USER,0L,NULL,0L);

			//加载道具
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_GAME_PROPERTY,0L,NULL,0L);

			//加载消息
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);

			//加载比赛时间配置信息
			//m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_MATCH_CONFIG,0L,NULL,0L);

			//加载比赛奖励配置信息
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_MATCH_REWARD_CONFIG,0L,NULL,0L);

			return true;
		}
	}

	return false;
}

//调度事件
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//时间事件
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	//调度时间
	if ((dwTimerID>=IDI_MAIN_MODULE_START)&&(dwTimerID<=IDI_MAIN_MODULE_FINISH))
	{
		//时间处理
		switch (dwTimerID)
		{
		case IDI_LOAD_ANDROID_USER:		//加载机器
			{
				//加载机器
				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_ANDROID_USER,0L,NULL,0L);

				return true;
			}
		case IDI_REPORT_SERVER_INFO:	//房间信息
			{
				//变量定义
				CMD_CS_C_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//设置变量
				ServerOnLine.dwOnLineCount=m_ServerUserManager.GetUserItemCount();

				//发送数据
				m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine));

				return true;
			}
		case IDI_CONNECT_CORRESPOND:	//连接协调
			{
				//发起连接
				tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
				m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//构造提示
				TCHAR szString[512]=TEXT("");
				_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//提示消息
				CTraceService::TraceString(szString,TraceLevel_Normal);

				return true;
			}
		case IDI_GAME_SERVICE_PULSE:	//服务维护
			{
				return true;
			}
		case IDI_DISTRIBUTE_ANDROID:	//分配机器
			{
				TCHAR szString9[256]=TEXT("");

				//动作处理
				if (m_AndroidUserManager.GetAndroidCount()>0 && m_pGameServiceOption->wServerType!=GAME_GENRE_MATCH)
				{
					//变量定义；动态加入、允许陪玩、允许占位、防作弊
					bool bAllowDynamicJoin=CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule);
					bool bAllowAndroidAttend=CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule);
					bool bAllowAndroidSimulate=CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule);
					bool bAllowAvertCheatMode=(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR));

					//add
					int nTableCount=m_pGameServiceOption->wTableCount;//*2/3;

					//模拟处理
					if (bAllowAndroidSimulate==true && bAllowAvertCheatMode==false)
					{
						//机器状态
						tagAndroidUserInfo AndroidSimulate; 
						m_AndroidUserManager.GetAndroidUserInfo(AndroidSimulate,ANDROID_SIMULATE);

						//最终坐下桌号
						UINT nSitTable=-1;

						//机器处理
						if (AndroidSimulate.wFreeUserCount>0)
						{
							for (WORD i=0;i<8;i++)
							{
								//选取要坐满的桌子
								UINT nRandProb = rand()%3; //有三分之二的概率做入满桌
								if (nRandProb != 0)
								{
									for (UINT m=0; m<sizeof(m_PrecedenceTableArray)/m_PrecedenceTableArray[0]; m++)
									{
										//桌子状况
										tagTableUserInfo TableUserInfoTemp;
										if (m_TableFrameArray[m_PrecedenceTableArray[m]] == NULL)
										{
											continue;	
										}

										WORD wUserSitCount=wUserSitCount=m_TableFrameArray[m_PrecedenceTableArray[m]]->GetTableUserInfo(TableUserInfoTemp);;
										if(TableUserInfoTemp.wTableAndroidCount<m_TableFrameArray[m_PrecedenceTableArray[m]]->GetChairCount())
										{
											nSitTable=m_PrecedenceTableArray[m];
											break;
										}
									}
								}

								//按区域选取桌子
								nRandProb = rand()%5; //有5分之一的概率随机插入一个机器人
								if ((nSitTable == -1) && (nRandProb != 0) && (m_RandPartNum != 1))
								{
									//划分区域数
									UINT nSectionNum=m_RandPartNum;
	
									//每区域桌子数
									UINT nTableCntPerSection=nTableCount/nSectionNum;
	
									//最小可插入机器人桌号
									UINT nMinTable=nTableCount;
	
									//插入机器人区域;
									UINT nInsSection=1;

									for (UINT j=0; j < nTableCount; j++)
									{
										if ((m_TableFrameArray[j]->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) 
										{
											m_TableFrameArray[j]->SetDynamicJoinAndriod(false);
											continue;
										}
	
										//桌子状况
										tagTableUserInfo TableUserInfoTemp;
										WORD wUserSitCount=m_TableFrameArray[j]->GetTableUserInfo(TableUserInfoTemp);
	
										//add
										if (TableUserInfoTemp.wTableAndroidCount>=m_TableFrameArray[j]->GetChairCount() * 2/3)
										{
											m_TableFrameArray[j]->SetDynamicJoinAndriod(false);
											continue;
										}
	
										//设置可插入机器人标志位
										m_TableFrameArray[j]->SetDynamicJoinAndriod(true);
	
											if (nMinTable>j)
											{
												nMinTable=j;
											}
									}

									for (UINT k=1; k<=nSectionNum; k++)
									{
										if(k*nTableCntPerSection>nMinTable)
										{
											nInsSection=k;
											break;
										}
									}
	
										//获取该区域可插入机器人桌子数目
										UINT nJoinAndroidTableCnt=0;
										for (UINT l=(nTableCntPerSection * (nInsSection-1)); l < nTableCntPerSection * nInsSection; l++)
										{
											if (m_TableFrameArray[l]->GetDynamicJoinAndriod() == true)
											{
												nJoinAndroidTableCnt++;
											}
										}
	
										//随机数=rand()%每份大小+1;
										UINT nRandNum=rand()%nJoinAndroidTableCnt + 1;
	
										UINT nLoopCnt=0;
										for (UINT l=(nTableCntPerSection * (nInsSection-1)); l < nTableCntPerSection * nInsSection; l++)
										{
											if (m_TableFrameArray[l]->GetDynamicJoinAndriod() == true)
											{
												nRandNum--;
												if (nRandNum==0)
												{
													nSitTable=l;
													break;
												}
											}
										}
	
										if (nSitTable==-1)
										{
											return true;
										}
								}
								else if((nSitTable == -1))
								{
									//随机桌子
									nSitTable=rand()%(__max(nTableCount,1));

									//获取桌子
									CTableFrame * pTableFrame=m_TableFrameArray[nSitTable];
									if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

									//桌子状况
									tagTableUserInfo TableUserInfo;
									WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

									//add
									if (TableUserInfo.wTableAndroidCount>=pTableFrame->GetChairCount()/2) continue;//不大于一半椅子
								}

								//获取桌子
								CTableFrame * pTableFrame=NULL;

								//桌子状况
								tagTableUserInfo TableUserInfo;

								if (nSitTable != -1)
								{
									pTableFrame=m_TableFrameArray[nSitTable];
									pTableFrame->GetTableUserInfo(TableUserInfo);
								}
								else
								{
									return true;
								}

								//坐下判断
								if (AndroidSimulate.wFreeUserCount>=TableUserInfo.wMinUserCount)
								{
									//_sntprintf(szString9,CountArray(szString9),TEXT("21*****"));
									//CTraceService::TraceString(szString9,TraceLevel_Normal);

									//变量定义
									WORD wHandleCount=0;
									WORD wWantAndroidCount=TableUserInfo.wMinUserCount;

									//数据调整
									if (m_pGameServiceAttrib->wChairCount>TableUserInfo.wMinUserCount)
									{
										WORD wChairCount=m_pGameServiceAttrib->wChairCount-2;//不能坐满
										WORD wFreeUserCount=AndroidSimulate.wFreeUserCount;
										WORD wOffUserCount=__min(wChairCount,wFreeUserCount)-TableUserInfo.wMinUserCount;
										wWantAndroidCount+=(wOffUserCount > 0) ? (rand()%wOffUserCount) : 0;
									}

									//坐下处理
									for (WORD j=0;j<AndroidSimulate.wFreeUserCount;j++)
									{
										//变量定义
										WORD wChairID=pTableFrame->GetRandNullChairID();

										//无效过滤
										//ASSERT(wChairID!=INVALID_CHAIR);
										if (wChairID==INVALID_CHAIR) continue;

										//用户坐下
										IAndroidUserItem * pIAndroidUserItem=AndroidSimulate.pIAndroidUserFree[j];

										if (pIAndroidUserItem!=NULL && pIAndroidUserItem->GetMeUserItem()->GetMemberOrder()==5) continue;

										if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true)
										{
											//_sntprintf(szString9,CountArray(szString9),TEXT("14*****"));
											//CTraceService::TraceString(szString9,TraceLevel_Normal);
											//设置变量
											wHandleCount++;

											//完成判断
											if (wHandleCount>=wWantAndroidCount) 
											{
												return true;
											}
										}
									}
									if(wHandleCount > 0) return true;
								}
							}
						}
					}

					//起立处理

					//四分之一的概率有人起立
					UINT nRandStandUp=rand()%4;

					if (nRandStandUp != 0)
					{
						WORD wStandUpCount=0;
						WORD wRandCount=(rand()%3+1);
						INT_PTR nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));

						for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
						{
							//获取桌子
							INT_PTR nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));
							INT_PTR nTableIndex=nIndex;
							CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
							//if (pTableFrame->IsGameStarted()==true) continue;//游戏已经开始

							//桌子状况
							tagTableUserInfo TableUserInfo;
							WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

							//用户过虑
							if (TableUserInfo.wTableAndroidCount==0) continue;
							if ((TableUserInfo.wTableUserCount>0)&&(bAllowAndroidAttend==true)) continue;

							//起立处理
							for (WORD j=0;j<pTableFrame->GetChairCount();j++)
							{
								//获取用户
								IServerUserItem * pIServerUserItem=pTableFrame->GetTableUserItem(j);
								if (pIServerUserItem==NULL) continue;

								//用户起立
								if ((pIServerUserItem->IsAndroidUser()==true)&&(pTableFrame->PerformStandUpAction(pIServerUserItem)==true))
								{
									wStandUpCount++;
									if(wStandUpCount>=wRandCount)
										return true;
									else
										break;
								}
							}
						}
					}

				}

				return true;
			}
		case IDI_DISTRIBUTE_PRECETABLE:
			{
				AndriodSitTableAssign();
			}
		case IDI_DBCORRESPOND_NOTIFY: //缓存定时处理
			{
				if(m_pIDBCorrespondManager) m_pIDBCorrespondManager->OnTimerNotify();
				return true;
			}
		case IDI_LOAD_SYSTEM_MESSAGE: //系统消息
			{
				//清除消息数据
				ClearSystemMessageData();

				//加载消息
				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);
				return true;
			}
		case IDI_LOAD_SENSITIVE_WORD:	//加载敏感词
			{
				//投递请求
				m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_SENSITIVE_WORDS,0,NULL,0);				
				return true;
			}
		case IDI_SEND_SYSTEM_MESSAGE: //系统消息
			{
				//数量判断
				if(m_SystemMessageList.GetCount()==0) return true;

				//时效判断
				DWORD dwCurrTime = (DWORD)time(NULL);
				POSITION pos = m_SystemMessageList.GetHeadPosition();
				while(pos != NULL)
				{
					POSITION tempPos = pos;
					tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);
					if(pTagSystemMessage->dwLastTime+pTagSystemMessage->SystemMessage.dwTimeRate < dwCurrTime)
					{
						//更新数据
						pTagSystemMessage->dwLastTime=dwCurrTime;

						//构造消息
						CMD_GR_SendMessage SendMessage = {};
						SendMessage.cbAllRoom = FALSE;
						SendMessage.cbGame = (pTagSystemMessage->SystemMessage.cbMessageType==1)?TRUE:FALSE;
						SendMessage.cbRoom = (pTagSystemMessage->SystemMessage.cbMessageType==2)?TRUE:FALSE;
						if(pTagSystemMessage->SystemMessage.cbMessageType==3)
						{
							SendMessage.cbGame = TRUE;
							SendMessage.cbRoom = TRUE;
						}
						lstrcpyn(SendMessage.szSystemMessage,pTagSystemMessage->SystemMessage.szSystemMessage,CountArray(SendMessage.szSystemMessage));
						SendMessage.wChatLength = lstrlen(SendMessage.szSystemMessage)+1;

						//发送消息
						WORD wSendSize = sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+CountStringBuffer(SendMessage.szSystemMessage);
						SendSystemMessage(&SendMessage,wSendSize);
					}
				}


				return true;
			}
		case IDI_DISTRIBUTE_USER: //分配用户
			{
				DistributeUserGame();
				return true;
			}
		case IDI_CHECK_STOCK:
			{
				tagDBR_GR_CheckStock lCheckStock;
				tagStockInfo* pStockInfo = &m_TotalStock;
				lCheckStock.lRoomID = m_pGameServiceOption->wServerID;
				lCheckStock.lTableID = -1;
				lCheckStock.lTypeID = pStockInfo->mType;
				lCheckStock.lStockScore = pStockInfo->mStockScore;
				lCheckStock.lRevenue = pStockInfo->mRevenue;
				lCheckStock.lbLoad = pStockInfo->m_bLoad ? FALSE : TRUE;
				lCheckStock.lCaiJin = pStockInfo->mCaiJin;
				lCheckStock.lCaiJinSent = pStockInfo->mCaiJinSent;

				m_pIDBCorrespondManager->PostDataBaseRequest(0L, DBR_GR_CHECKSTOCK, 0, &lCheckStock, sizeof(lCheckStock));

				pStockInfo = &m_RobotStock;
				lCheckStock.lRoomID = m_pGameServiceOption->wServerID;
				lCheckStock.lTableID = -1;
				lCheckStock.lTypeID = pStockInfo->mType;
				lCheckStock.lStockScore = pStockInfo->mStockScore;
				lCheckStock.lRevenue = pStockInfo->mRevenue;
				lCheckStock.lbLoad = pStockInfo->m_bLoad ? FALSE : TRUE;
				lCheckStock.lCaiJin = pStockInfo->mCaiJin;
				lCheckStock.lCaiJinSent = pStockInfo->mCaiJinSent;

				m_pIDBCorrespondManager->PostDataBaseRequest(0L, DBR_GR_CHECKSTOCK, 0, &lCheckStock, sizeof(lCheckStock));
			}
			break;
		case IDI_GET_USER_LUCKY:
			{
				m_pIDBCorrespondManager->PostDataBaseRequest(0L, DBR_GR_GET_USER_LUCKY,0,NULL,0);

				CString strSQL=TEXT("");
				for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
				{
					CTableFrame * pTableFrame=m_TableFrameArray[i];
					if (NULL==pTableFrame) continue;
					if (!pTableFrame->IsGameStarted()) continue;

					for (WORD j=0; j<pTableFrame->GetChairCount(); j++)
					{
						IServerUserItem * pServerUserItem=pTableFrame->GetTableUserItem(j);
						if (NULL==pServerUserItem) continue;
						if (pServerUserItem->IsAndroidUser()) continue;
						CString str;
						DWORD dwUserID=pServerUserItem->GetUserID();
						LONG lInsideScore=pTableFrame->GetInsideScore(j);
						str.Format(TEXT("exec GSP_GR_SaveInsideScore %d,%d;"),dwUserID,lInsideScore);

						if (strSQL.GetLength()>=1023 - 50)
						{
							DBR_GR_Sentence Sentence={0};
							_tcsncpy(Sentence.szSQL,strSQL,CountArray(Sentence.szSQL));
							m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_SENTENCE,0,&Sentence,sizeof(DBR_GR_Sentence));
							strSQL=TEXT("");
						}
						strSQL+=str;
					}
				}
				if (strSQL!=TEXT(""))
				{
					DBR_GR_Sentence Sentence={0};
					_tcsncpy(Sentence.szSQL,strSQL,CountArray(Sentence.szSQL));
					m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_SENTENCE,0,&Sentence,sizeof(DBR_GR_Sentence));
				}
			}
			break;
		case IDI_CLOWN_FAKE_MSG:
			{
				// get the random fake name
				WORD wFakeNameSum = sizeof(g_pszFakeName)/sizeof(*g_pszFakeName);
				WORD wFNNum = rand()%wFakeNameSum;
				WORD wRewardNum = rand()%131 + 30;  // 30 ~ 160

				CMD_GR_CLOWNFAKEANNOUNCE stFakeAnnounce;
				ZeroMemory(&stFakeAnnounce, sizeof(CMD_GR_CLOWNFAKEANNOUNCE));
				_sntprintf(stFakeAnnounce.szClownFakeAnnounceMent,CountArray(stFakeAnnounce.szClownFakeAnnounceMent),
					TEXT("恭喜【%s】在超级海盗中赢得%ld万大奖。"),g_pszFakeName[wFNNum],wRewardNum);
				//CTraceService::TraceString(stFakeAnnounce.szClownFakeAnnounceMent,TraceLevel_Normal);

				m_pITCPNetworkEngine->SendDataBatch(MDM_GR_MANAGE,SUB_GR_CLOWN_FAKEANNOUNCEMENT,(VOID *)&stFakeAnnounce,sizeof(CMD_GR_CLOWNFAKEANNOUNCE),BG_COMPUTER);	
				
			}
			break;
		case IDI_READ_CONFIG:
			{
				//读取配置
				TCHAR szPath[MAX_PATH]=TEXT("");
				TCHAR szConfigFileName[MAX_PATH] = TEXT("");
				GetCurrentDirectory(sizeof(szPath),szPath);
				_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\GameServerTimerInterval.ini"),szPath);

				//读取配置
				TCHAR szServerName[MAX_PATH]=TEXT("");
				WORD wFakeAnnounceTimeInterval = 60000;
				wFakeAnnounceTimeInterval = GetPrivateProfileInt(TEXT("TimerInterval"),TEXT("FakeAnnounceTimeInterval"),60000,szConfigFileName);
				TCHAR sBigRewardBaseNum[100];
				TCHAR sBigRewardMaxNum[100];
				GetPrivateProfileString(TEXT("TimerInterval"),TEXT("BigRewardBaseNum"),"2166650458",sBigRewardBaseNum,LEN_SERVER,szConfigFileName);
				GetPrivateProfileString(TEXT("TimerInterval"),TEXT("BigRewardMaxNum"),"8000000000",sBigRewardMaxNum,LEN_SERVER,szConfigFileName);
				sscanf(sBigRewardBaseNum, "%lld", &m_lBigRewardBaseNum);
				sscanf(sBigRewardMaxNum, "%lld", &m_lBigRewardMaxNum);
				m_lBigRewardAddBaseNum = GetPrivateProfileInt(TEXT("TimerInterval"),TEXT("BigRewardAddBaseNum"),100,szConfigFileName);
				m_lBigRewardAddRandNum = GetPrivateProfileInt(TEXT("TimerInterval"),TEXT("BigRewardAddRandNum"),900,szConfigFileName);
				if (wFakeAnnounceTimeInterval != m_wFakeAnnounceTimeInterval)
				{	
					m_wFakeAnnounceTimeInterval = wFakeAnnounceTimeInterval;
					m_pITimerEngine->KillTimer(IDI_CLOWN_FAKE_MSG);
					m_pITimerEngine->SetTimer(IDI_CLOWN_FAKE_MSG,m_wFakeAnnounceTimeInterval,(DWORD)(-1),NULL);
				}


			}
			break;
		case IDI_CLOWN_BIG_REWARD:
			{

				CMD_GR_CLOWNBIGREWARD stBigReward;
				ZeroMemory(&stBigReward, sizeof(stBigReward));
				if (m_lBigRewardAddRandNum <= 0)
				{
					m_lBigRewardAddRandNum =1;
				}
				m_lBigReward = m_lBigReward + rand()%m_lBigRewardAddRandNum + m_lBigRewardAddBaseNum;
				if (m_lBigReward > m_lBigRewardMaxNum)
				{
					m_lBigReward = m_lBigRewardBaseNum;
				}
				stBigReward.lBigReward = m_lBigReward;
				m_pITCPNetworkEngine->SendDataBatch(MDM_GR_MANAGE,SUB_GR_CLOWN_BIG_REWARD,(VOID *)&stBigReward,sizeof(CMD_GR_CLOWNBIGREWARD),BG_COMPUTER);
			}
			break;
		//case IDI_READ_MATCH_CONFIG:
		//	{
		//		//读取配置
		//		TCHAR szPath[MAX_PATH]=TEXT("");
		//		TCHAR szConfigFileName[MAX_PATH] = TEXT("");
		//		GetCurrentDirectory(sizeof(szPath),szPath);
		//		_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\GameServerMatchConfig.ini"),szPath);

		//		//读取配置
		//		TCHAR szServerName[MAX_PATH]=TEXT("");
		//		m_nSignUpCost = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("SignUpCost"),10000,szConfigFileName);
		//		m_nTeamMemberNum = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("TeamMemberNum"),8,szConfigFileName);
		//		m_nMessageRemindTime1 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime1"),5,szConfigFileName);
		//		m_nMessageRemindTime2 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime2"),3,szConfigFileName);
		//		m_nMessageRemindTime3 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime3"),1,szConfigFileName);
		//		m_nMatchBeginRemindTime = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MatchBeginRemindTime"),5,szConfigFileName);
		//	}
		//	break;
		//case IDI_TIMER_CHECK_INTERVAL:
		//	{
		//		SYSTEMTIME st;
		//		GetLocalTime(&st);
		//		if (st.wHour==24&&st.wMinute==0)
		//		{
		//			//加载比赛配置
		//			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_MATCH_CONFIG,0L,NULL,0L);
		//		}
		//		tagMatchConfigItem sMatchConfigItem;
		//		GetNextMatchInfo(sMatchConfigItem);
		//		int nMatchStartTimeHour = sMatchConfigItem.nStartTime/100;
		//		int nMatchStartTimeMinute = sMatchConfigItem.nStartTime%100;
		//		if (nMatchStartTimeHour==st.wHour&&nMatchStartTimeMinute-st.wMinute==m_nMessageRemindTime1)
		//		{
		//			//发送提示信息
		//		}
		//		else if (nMatchStartTimeHour==st.wHour&&nMatchStartTimeMinute-st.wMinute==m_nMessageRemindTime2)
		//		{
		//			//发送提示信息
		//		}
		//		if (nMatchStartTimeHour==st.wHour&&nMatchStartTimeMinute-st.wMinute==m_nMessageRemindTime3)
		//		{
		//			//发送提示信息
		//			m_pITimerEngine->SetTimer(IDI_MATCH_BEGIN_REMINDER,1000,(DWORD)(-1),NULL);

		//		}
		//		
		//	}
		//	break;
		//case IDI_MATCH_BEGIN_REMINDER:
		//	{
		//		//发送提示信息
		//	}
		//	break;
	case IDI_MATCH_SIGN_UP:
			{
				// Get Current Time
				//if ()  // Current Time equal Begin Time
				//{
				//	m_bMatchSignUpMark = true;	
				//}
				//else if ()  // Current Time equal End Time
				//{
				//	m_bMatchSignUpMark = false;	
				//}
				//else
				//{
				//	break;
				//}

			}

			break;
		//case IDI_MATCH_TIME:
		//	{
				//if () // match begin
				//{
				//	m_bIsMatchTime = true;
				//m_pITimerEngine->SetTimer(IDI_MATCH_GET_USERSCORE, TIME_MATCH_GET_USERSCORE_INTERVAL, (DWORD)(-1), NULL);


				//}

				//else if ()
				//{
				//	m_bIsMatchTime = false;
				//	// send
				//}
				//else
				//{
				//	break;
				//}
			//	return true;
			//}
			//break;
		case IDI_MATCH_GET_USERSCORE:
			{
				if (m_nCurrentMatchID == 0)
				{
					printf ("Invalid Current Match ID\n");
					return false;
				}

				printf("Upload Score to Timer Begin\n");

				CMD_CS_C_MatchUserScore stMatchUserInfoCollection = {0};
				stMatchUserInfoCollection.wServerID = m_pGameServiceOption->wServerID;

				WORD wCurrentUserCnt = 0;
				WORD wUserCnt = 0;
				for(int i = 0; i < m_vMatchUserInfo[m_nCurrentMatchID-1].size(); i++)
				{
					stMatchUserInfoCollection.aMatchUserInfo[i].dwUserID = m_vMatchUserInfo[m_nCurrentMatchID - 1][i].dwUserID;
					stMatchUserInfoCollection.aMatchUserInfo[i].lUserScore = m_vMatchUserInfo[m_nCurrentMatchID - 1][i].lUserScore;
					stMatchUserInfoCollection.aMatchUserInfo[i].wGroupID = m_vMatchUserInfo[m_nCurrentMatchID - 1][i].wGroupID;
					printf("Upload UserID is %ld\n", stMatchUserInfoCollection.aMatchUserInfo[i].dwUserID);
					wUserCnt ++;
				}

				stMatchUserInfoCollection.wUserCnt = wUserCnt;
				stMatchUserInfoCollection.wDataSize = sizeof(WORD) * 3 + sizeof(tagMatchUserInfo) * wUserCnt;

				printf("upload score datasize is %ld\n", stMatchUserInfoCollection.wDataSize);
				m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_MATCH_USERSCORE_UPDATE,
					&stMatchUserInfoCollection,stMatchUserInfoCollection.wDataSize);

				for (int i = 0 ; i < m_vMatchUserInfo[m_nCurrentMatchID - 1].size(); i ++)
				{
					m_vMatchUserInfo[m_nCurrentMatchID - 1][i].lUserScore = 0;		
				}
			}

			break;
		}
	}

	//机器时器
	if ((dwTimerID>=IDI_REBOT_MODULE_START)&&(dwTimerID<=IDI_REBOT_MODULE_FINISH))
	{
		//时间处理
		m_AndroidUserManager.OnEventTimerPulse(dwTimerID,wBindParam);

		return true;
	}

	//比赛定时器
	if((dwTimerID>=IDI_MATCH_MODULE_START)&&(dwTimerID<IDI_MATCH_MODULE_FINISH))
	{
		if(m_pIGameMatchServiceManager!=NULL) m_pIGameMatchServiceManager->OnEventTimer(dwTimerID,wBindParam);
		return true;
	}

	//桌子时间
	if ((dwTimerID>=IDI_TABLE_MODULE_START)&&(dwTimerID<=IDI_TABLE_MODULE_FINISH))
	{
		//桌子号码
		DWORD dwTableTimerID=dwTimerID-IDI_TABLE_MODULE_START;
		WORD wTableID=(WORD)(dwTableTimerID/TIME_TABLE_MODULE_RANGE);

		//时间效验
		if (wTableID>=(WORD)m_TableFrameArray.GetCount()) 
		{
			ASSERT(FALSE);
			return false;
		}

		//时间通知
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		return pTableFrame->OnEventTimer(dwTableTimerID%TIME_TABLE_MODULE_RANGE,wBindParam);
	}

	return false;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GR_LOGON_SUCCESS:			//登录成功
		{
			return OnDBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GR_LOGON_FAILURE:			//登录失败
		{
			return OnDBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GAME_PARAMETER:			//游戏参数
		{
			return OnDBGameParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GAME_COLUMN_INFO:		//列表信息
		{
			return OnDBGameColumnInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GR_GAME_ANDROID_INFO:		//机器信息
		{
			return OnDBGameAndroidInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GAME_PROPERTY_INFO:		//道具信息
		{
			return OnDBGamePropertyInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_INFO:		//银行信息
		{
			return OnDBUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_SUCCESS:	//银行成功
		{
			return OnDBUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_TRANSER_DRAGON:
		{
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			SendData(pIServerUserItem, MDM_GR_INSURE, SUB_GRO_USER_TRANSER_DRAGON, pData, wDataSize);
			return true;
		}
	case DBO_GR_USER_WIN_DRAGON_PIECE:
		{
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			SendData(pIServerUserItem, MDM_GR_INSURE, SUB_GRO_USER_WIN_DRAGON_PIECE, pData, wDataSize);
			return true;
		}
	case DBO_GR_WIN_PEARL:
		{
			DBR_GO_WinPearl * pWinPearl = (DBR_GO_WinPearl *)pData;
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			if (pIServerUserItem != NULL && pIServerUserItem->GetTableID() != INVALID_TABLE)
			{
				m_TableFrameArray[pIServerUserItem->GetTableID()]->OnWinPearl(pIServerUserItem, pWinPearl->nPearlType, pWinPearl->nPearlCount);
			}

			//CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
			//VIPUpgrade.cbType = 40;
			//LPCTSTR pPearlType[] =
			//{
			//	TEXT("初级珍珠"), TEXT("中级珍珠"), TEXT("高级珍珠"),
			//};

			//if (pWinPearl->nPearlType < _countof(pPearlType))
			//{
			//	_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】在#ff0000%s#ffffff幸运的捕获#ff0000%s#ffffff一颗。", pIServerUserItem->GetNickName(), m_pGameServiceOption->szServerName, pPearlType[pWinPearl->nPearlType]);

			//	if (m_pITCPSocketService)
			//	{
			//		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
			//	}
			//}

			return true;
		}
	case DBO_GET_USER_ITEM_COUNT:
				{
printf("DBO_GET_USER_ITEM_COUNT\n");
			DBO_GetUserItemCount * pGetUserItemCount = (DBO_GetUserItemCount *)pData;
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			if (pIServerUserItem != NULL && pIServerUserItem->GetTableID() != INVALID_TABLE)
			{
				m_TableFrameArray[pIServerUserItem->GetTableID()]->OnGetUserItemCount(pIServerUserItem, pGetUserItemCount->nType, pGetUserItemCount->lCount);
				}

			return true;
		}
	case DBO_GR_USER_INSURE_TRANSFER_INFO:
		{
			return OnDBUserInsureTransferInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_FAILURE:	//银行失败
		{
			return OnDBUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_USER_INFO:  //用户信息
		{
			return OnDBUserInsureUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_PROPERTY_SUCCESS:		//道具成功
		{
			return OnDBPropertySuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GR_SYSTEM_MESSAGE_RESULT:  //系统消息
		{
			return OnDBSystemMessage(dwContextID,pData,wDataSize);
		}
	case DBO_GR_SENSITIVE_WORDS:	//加载敏感词
		{
			return OnDBSensitiveWords(dwContextID,pData,wDataSize);
		}
	case DBR_GR_CHECKSTOCK_SUCCESS:
		{
			//效验参数
			ASSERT(wDataSize==sizeof(tagDBR_GR_CheckStockSuccess));
			if (wDataSize!=sizeof(tagDBR_GR_CheckStockSuccess)) return false;

			//消息处理
			tagDBR_GR_CheckStockSuccess * pCheckStock =(tagDBR_GR_CheckStockSuccess *)pData;

			if (pCheckStock)
			{
				if (pCheckStock->lType == m_TotalStock.mType)
				{
					if (m_TotalStock.m_bLoad)
					{
						m_TotalStock.mStockScore += pCheckStock->lStockChange;
						m_TotalStock.mRevenue = 0;
					}
					else
					{
						m_TotalStock.mRoomID = m_pGameServiceOption->wServerID;
						m_TotalStock.mTableID = -1;
						m_TotalStock.mStockScore = pCheckStock->lStockScore;
						m_TotalStock.mCaiJin = pCheckStock->lCaiJin;
						m_TotalStock.mCaiJinSent = pCheckStock->lCaiJinSent;
						m_TotalStock.m_bLoad = true;
					}
				}

				if (pCheckStock->lType == m_RobotStock.mType)
				{
					if (m_RobotStock.m_bLoad)
					{
						m_RobotStock.mStockScore += pCheckStock->lStockChange;
						m_RobotStock.mRevenue = 0;
					}
					else
					{
						m_RobotStock.mRoomID = m_pGameServiceOption->wServerID;
						m_RobotStock.mTableID = -1;
						m_RobotStock.mStockScore = pCheckStock->lStockScore;
						m_RobotStock.mCaiJin = pCheckStock->lCaiJin;
						m_RobotStock.mCaiJinSent = pCheckStock->lCaiJinSent;
						m_RobotStock.m_bLoad = true;
					}
				}

			}

			return true;
		}
	case DBR_GET_USER_LUCKY_SUCCESS:
		{
			return OnDBGetUserLucky(dwContextID,pData,wDataSize);
		}
	case DBR_LOADUSERITEM_SUCCESS:
		{
			if (wDataSize!=sizeof(tagUserItem)) return false;

			tagUserItem * pItem=(tagUserItem *)pData;

			tagBindParameter * pConnectItemInfo=GetBindParameter(LOWORD(dwContextID));
			if (pConnectItemInfo==NULL) return true;
			if (pConnectItemInfo->dwSocketID!=dwContextID) return true;

			IServerUserItem * pIServerUserItem = m_ServerUserManager.SearchUserItem(pItem->nUserID);
			if (pIServerUserItem!=NULL)
			{
				pIServerUserItem->AddUserItem(pItem);
			}

			return true;
		}
	case DBO_GR_PRESENT_SCORE:
		{
			if (wDataSize!=sizeof(DBO_GR_PresentScore)) return false;
			DBO_GR_PresentScore * pPresentScore=(DBO_GR_PresentScore *)pData;

			CString szMessage;
			szMessage.Format(TEXT("恭喜玩家[%s]在[%s]房间，获得泡点分数%d分。"),pPresentScore->szNickName,pPresentScore->szServerName,pPresentScore->nScore);
			//构造消息
			CMD_GR_SendMessage SendMessage={0};
			SendMessage.cbAllRoom=FALSE;
			SendMessage.cbRoom=TRUE;
			SendMessage.cbGame=FALSE;
			lstrcpyn(SendMessage.szSystemMessage,szMessage.GetBuffer(),CountArray(SendMessage.szSystemMessage));
			SendMessage.wChatLength=lstrlen(SendMessage.szSystemMessage)+1;
			WORD wSendSize=sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+CountStringBuffer(SendMessage.szSystemMessage);
			SendSystemMessage(&SendMessage,wSendSize);

			return true;
		}
	case DBO_GR_CHECK_TRUMPET:
		{
			if (wDataSize!=sizeof(DBR_GR_CheckTrumpet)) return false;
			DBR_GR_CheckTrumpet * pCheckTrumpet = (DBR_GR_CheckTrumpet *)pData;

			//获取用户
			WORD wBindIndex=LOWORD(dwContextID);
			IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

			if (pCheckTrumpet->lReturnValue == 1)
			{
				//发送消息
				SendPropertyFailure(pIServerUserItem,TEXT("对不起，您的会员等级不足，不能发送大喇叭。"), 0L, pCheckTrumpet->cbRequestArea);
				return true;
			}
			if (pCheckTrumpet->lReturnValue == 2)
			{
				//发送消息
				SendPropertyFailure(pIServerUserItem,TEXT("对不起，您的大喇叭数量不足，不能发送。"), 0L, pCheckTrumpet->cbRequestArea);
				return true;
			}

			////道具索引
			//BYTE cbIndex=(pCheckTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?2:3;

			//构造结构
			CMD_GR_S_SendTrumpet  SendTrumpet;
			SendTrumpet.dwSendUserID=pCheckTrumpet->dwUserID;
			SendTrumpet.wPropertyIndex=pCheckTrumpet->wPropertyIndex;
			SendTrumpet.TrumpetColor=pCheckTrumpet->TrumpetColor;
			ZeroMemory(SendTrumpet.szTrumpetContent,sizeof(SendTrumpet.szTrumpetContent));
			CopyMemory(SendTrumpet.szSendNickName,pIServerUserItem->GetNickName(),sizeof(SendTrumpet.szSendNickName));

			//字符过滤
			SensitiveWordFilter(pCheckTrumpet->szTrumpetContent,SendTrumpet.szTrumpetContent,CountArray(SendTrumpet.szTrumpetContent));

			////房间转发
			//if(cbIndex==3)
			{
				//广播房间
				WORD wUserIndex=0;
				CMD_CS_S_SendTrumpet * pSendTrumpet = (CMD_CS_S_SendTrumpet *)&SendTrumpet;
				if(m_pITCPSocketService)
				{
					m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIP_TRUMPET,pSendTrumpet,sizeof(CMD_CS_S_SendTrumpet));
				}
			}

			////游戏转发
			//if(cbIndex==2)
			//{
			//	//发送数据
			//	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_S_VIP_TRUMPET,&SendTrumpet,sizeof(SendTrumpet),BG_COMPUTER);
			//}

			return true;
		}
	case DBO_GR_QUERY_SKILL:
		{
			CMD_GRO_QuerySkill * pQuerySkill = (CMD_GRO_QuerySkill *)pData;
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			pIServerUserItem->SetCannonStatus(pQuerySkill->nCannonStatus);
			SendData(pIServerUserItem, MDM_GR_SKILL, SUB_GRO_QUERY_SKILL, pData, wDataSize);
			return true;
		}
	case DBO_GR_USE_SKILL:
		{
			DBO_GR_UseSkill * pUseSkill = (DBO_GR_UseSkill *)pData;
			CMD_GRO_UseSkill UseSkill = {0};
			UseSkill.cbSkillID = pUseSkill->cbSkillID;
			UseSkill.nCount = pUseSkill->nCount;
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			SendData(pIServerUserItem, MDM_GR_SKILL, SUB_GRO_USE_SKILL, &UseSkill, sizeof(UseSkill));
			return true;
		}
	case DBO_GR_SKILL_GIVE:
		{
			DBO_GR_GiveSkill * pGiveSkill = (DBO_GR_GiveSkill *)pData;
			CMD_GRO_GiveSkill GiveSkill = {0};
			GiveSkill.cbSkillID = pGiveSkill->cbSkillID;
			GiveSkill.nCount = pGiveSkill->nCount;
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			SendData(pIServerUserItem, MDM_GR_SKILL, SUB_GRO_GIVE_SKILL, &GiveSkill, sizeof(GiveSkill));
			return true;
		}
	case DBO_GR_NEW_GUID_GIFT:
		{
			DBO_GR_NewGuidGift * pNewGuidGift = (DBO_GR_NewGuidGift *)pData;
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

			if (pIServerUserItem != NULL && pIServerUserItem->GetTableID() != INVALID_TABLE)
			{
				m_TableFrameArray[pIServerUserItem->GetTableID()]->OnNewGuidGift(pIServerUserItem, pNewGuidGift->bSuccess, pNewGuidGift->nCannonStatus, pNewGuidGift->nLotteryCount);
			}

			return true;
		}
	case DBO_GR_UPDATE_TREASUREMAP:
		{
			DBO_GR_Update_Treasuremap * pResult = (DBO_GR_Update_Treasuremap *)pData;
			if (pResult == NULL)
			{
				return true;
			}
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			pIServerUserItem->SetTreasureMapCount(pResult->nCount);
			return true;
		}
	case DB0_GR_FIRST_ENTER_SUPER:
		{
			DBO_GR_First_Enter_Super * pInfo = (DBO_GR_First_Enter_Super *)pData;
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			pIServerUserItem->WriteUserScore((LONGLONG)pInfo->nRewardCoin, 0, 0, 0, SCORE_TYPE_WIN, 0);
			return true;
		}
	case DBO_GR_UPDATE_KVALUEANDINTEGRATION:
		{
			DBO_GR_Update_KValueAndIntegration * pResult = (DBO_GR_Update_KValueAndIntegration *)pData;
			if (pResult == NULL)
			{
				return true;
			}
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			printf("CAttemperEngineSink,USerId=%d,KValue=%d ,Integration=%d\n",pIServerUserItem->GetUserID(),pResult->nKValue,pResult->nIntegration);
			pIServerUserItem->UpdateUserKValueAndIntegration(pResult->nKValue,1);
			return true;
		}
	case DBO_GR_UPDATE_MAXKVALUE:
		{
			DBO_GR_Update_MaxKValue * pResult = (DBO_GR_Update_MaxKValue *)pData;
			if (pResult == NULL)
			{
				return true;
			}
			IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));
			pIServerUserItem->UpdateUserMaxKValue(pResult->nCount);
			return true;
		}
	//case DBO_LOAD_MATCH_CONFIG:
	//	{
			//ASSERT(wDataSize%sizeof(DBO_Load_Match_Config_Item)==0);
			//if (wDataSize%sizeof(DBO_Load_Match_Config_Item)!=0) return false;

			////变量定义
			//WORD wItemCount=wDataSize/sizeof(DBO_Load_Match_Config_Item);
			//DBO_Load_Match_Config_Item * pGameItem=(DBO_Load_Match_Config_Item *)pData;
			//m_MatchConfigItemList.clear();
			////更新数据
			//for (WORD i=0;i<wItemCount;i++)
			//{
			//	//变量定义
			//	tagMatchConfigItem MatchConfigItem;
			//	ZeroMemory(&MatchConfigItem,sizeof(MatchConfigItem));

			//	//构造数据
			//	MatchConfigItem.nStartTime = pGameItem->nStartTime;
			//	MatchConfigItem.nEndTime = pGameItem->nEndTime;
			//	MatchConfigItem.nApplyCost = pGameItem->nApplyCost;
			//	MatchConfigItem.nGameTime = pGameItem->nGameTime;
			//	MatchConfigItem.nMatchNum = pGameItem->nMatchNum;
			//	MatchConfigItem.nMatchUserCount = pGameItem->nMatchUserCount;
			//	MatchConfigItem.nMatchType = pGameItem->nMatchType;
			//	//插入列表
			//	m_MatchConfigItemList.push_back(MatchConfigItem);
			//}
		//	return true;
		//}
	case DBO_GR_MATCH_USERSIGNUP:
		{
			OnUserMatchSignUPRes(dwContextID,pData,wDataSize);
			return true;
		}
	case DBO_LOAD_MATCH_REWARD_CONFIG:
		{
			ASSERT(wDataSize%sizeof(DBO_Load_Match_Reward_Config_Item)==0);
			if (wDataSize%sizeof(DBO_Load_Match_Reward_Config_Item)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(DBO_Load_Match_Reward_Config_Item);
			DBO_Load_Match_Reward_Config_Item * pGameItem=(DBO_Load_Match_Reward_Config_Item *)pData;
			m_MatchRewardConfigItemList.clear();
			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				//变量定义
				tagMatchRewardConfigItem MatchConfigItem;
				ZeroMemory(&MatchConfigItem,sizeof(MatchConfigItem));

				//构造数据
				MatchConfigItem.nRankStart = pGameItem->nRankStart;
				MatchConfigItem.nRankEnd = pGameItem->nRankEnd;
				MatchConfigItem.nMachType = pGameItem->nMachType;
				MatchConfigItem.nShareType = pGameItem->nShareType;

				for (int i=0;i<10;++i)
				{
					MatchConfigItem.nReward[i] = pGameItem->nReward[i];
				}
				//插入列表
				m_MatchRewardConfigItemList.push_back(MatchConfigItem);
			}
			return true;
		}
	case DBO_GR_MATCH_GET_PRIZE:
		{
			ASSERT(wDataSize%sizeof(DBO_GR_Match_Get_Prize)==0);
			if (wDataSize%sizeof(DBO_GR_Match_Get_Prize)!=0) return false;
			//变量定义
			DBO_GR_Match_Get_Prize * pDBMatchResult=(DBO_GR_Match_Get_Prize *)pData;
			CMD_GPO_MatchGetMatchPriseRes stMatchGetMatchPrizeRes = {0};
			stMatchGetMatchPrizeRes.bPriseStatus = pDBMatchResult->bPriseStatus;
			stMatchGetMatchPrizeRes.nPriseType = pDBMatchResult->nPriseType;
			stMatchGetMatchPrizeRes.nPriseCount = pDBMatchResult->nPriseCount;
			stMatchGetMatchPrizeRes.llUserScore = pDBMatchResult->lluserScore;
			stMatchGetMatchPrizeRes.nMatchId = pDBMatchResult->nMatchId;
			if (pDBMatchResult->bPriseStatus==true)
			{
				char szText[128];
				sprintf(szText,"领取成功，获得%d金币。",pDBMatchResult->nPriseCount);
				lstrcpyn(stMatchGetMatchPrizeRes.szDescription,szText,sizeof(stMatchGetMatchPrizeRes.szDescription));
			}
			else
			{
				char szText[128];
				sprintf(szText,"奖励已经领取",pDBMatchResult->nPriseCount);
				lstrcpyn(stMatchGetMatchPrizeRes.szDescription,szText,sizeof(stMatchGetMatchPrizeRes.szDescription));
			}
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GR_MATCH_GET_MATCH_PRIZE_RES, &stMatchGetMatchPrizeRes, sizeof(CMD_GPO_MatchGetMatchPriseRes));
			return true;
		}
	}

	//比赛事件
	if(wRequestID>=DBO_GR_MATCH_EVENT_START && wRequestID<=DBO_GR_MATCH_EVENT_END)
	{
		//参数效验
		if(m_pIGameMatchServiceManager==NULL) return false;

		tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
		IServerUserItem *pIServerUserItem=pBindParameter!=NULL?pBindParameter->pIServerUserItem:NULL;

		//废弃判断
		if ((pBindParameter->pIServerUserItem==NULL)||(pBindParameter->dwSocketID!=dwContextID))
		{
			//错误断言
			ASSERT(FALSE);
			return true;
		}

		return m_pIGameMatchServiceManager->OnEventDataBase(wRequestID,pIServerUserItem,pData,wDataSize);
	}

	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//设置变量
		m_bCollectUser=false;

		//删除时间
		m_pITimerEngine->KillTimer(IDI_REPORT_SERVER_INFO);

		//重连判断
		if (m_bNeekCorrespond==true)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("与协调服务器的连接关闭了，%ld 秒后将重新连接"),m_pInitParameter->m_wConnectTime);

			//提示消息
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);
		}

		return true;
	}

	return false;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//错误判断
		if (nErrorCode!=0)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("协调服务器连接失败 [ %ld ]，%ld 秒后将重新连接"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//提示消息
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//提示消息
		CTraceService::TraceString(TEXT("正在发送游戏房间注册信息..."),TraceLevel_Normal);

		//变量定义
		CMD_CS_C_RegisterServer RegisterServer;
		ZeroMemory(&RegisterServer,sizeof(RegisterServer));

		//服务端口
		CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
		RegisterServer.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();

		//构造数据
		RegisterServer.wKindID=m_pGameServiceOption->wKindID;
		RegisterServer.wNodeID=m_pGameServiceOption->wNodeID;
		RegisterServer.wSortID=m_pGameServiceOption->wSortID;
		RegisterServer.wServerID=m_pGameServiceOption->wServerID;
		RegisterServer.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		RegisterServer.dwFullCount=m_pGameServiceOption->wMaxPlayer-RESERVE_USER_COUNT;
		RegisterServer.lMinEnterScore=m_pGameServiceOption->lMinEnterScore;
		lstrcpyn(RegisterServer.szServerName,m_pGameServiceOption->szServerName,CountArray(RegisterServer.szServerName));
		lstrcpyn(RegisterServer.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterServer.szServerAddr));

		//发送数据
		ASSERT(m_pITCPSocketService!=NULL);
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_SERVER,&RegisterServer,sizeof(RegisterServer));

		//设置时间
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_REPORT_SERVER_INFO,TIME_REPORT_SERVER_INFO*1000L,TIMES_INFINITY,0);

		return true;
	}

	return true;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//注册服务
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//服务信息
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_USER_COLLECT:	//用户汇总
			{
				return OnTCPSocketMainUserCollect(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //管理服务
			{
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//设置变量
	if (pBindParameter!=NULL)
	{
		pBindParameter->dwSocketID=dwSocketID;
		pBindParameter->dwClientAddr=dwClientAddr;
		pBindParameter->dwActiveTime=(DWORD)time(NULL);

		return true;
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//获取用户
	IServerUserItem * pIServerUserItem=pBindParameter->pIServerUserItem;

	//用户处理
	if (pIServerUserItem!=NULL)
	{
		//变量定义
		WORD wTableID=pIServerUserItem->GetTableID();

		//断线处理
		if (wTableID!=INVALID_TABLE)
		{
			//解除绑定
			pIServerUserItem->DetachBindStatus();

			//断线通知
			ASSERT(wTableID<m_pGameServiceOption->wTableCount);
			m_TableFrameArray[wTableID]->OnEventUserOffLine(pIServerUserItem);
		}
		else
		{
			pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
		}
	}

	//清除信息
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));

	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	bool bRet=false;
	switch (Command.wMainCmdID)
	{
	case MDM_GR_USER:		//用户命令
		{
			bRet=OnTCPNetworkMainUser(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_LOGON:		//登录命令
		{
			bRet=OnTCPNetworkMainLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GF_GAME:		//游戏命令
		{
			bRet=OnTCPNetworkMainGame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GF_FRAME:		//框架命令
		{
			bRet=OnTCPNetworkMainFrame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_INSURE:		//银行命令
		{
			bRet=OnTCPNetworkMainInsure(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_MANAGE:		//管理命令
		{
			bRet=OnTCPNetworkMainManage(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_MATCH:		//比赛命令
		{
			bRet=OnTCPNetworkMainMatch(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_SKILL:
		{
			bRet=OnTCPNetworkMainSkill(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_KN_COMMAND:
		{
			//printf("SUB_KN_DETECT_MANUAL\n");
			if (Command.wSubCmdID == SUB_KN_DETECT_MANUAL)
			{
				m_pITCPNetworkEngine->SendData(dwSocketID, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL, NULL, 0);
			}
			return true;
		}
	case MDM_GR_GET_BIG_REWARD:
		{
			bRet=OnTCPNetworkMainSkill(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	}

	return true;
}

//房间消息
bool CAttemperEngineSink::SendRoomMessage(LPCTSTR lpszMessage, WORD wType)
{
	//变量定义
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//构造数据
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//数据属性
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//发送数据
	m_AndroidUserManager.SendDataToClient(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_COMPUTER);

	return true;
}

//游戏消息
bool CAttemperEngineSink::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//变量定义
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//构造数据
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//数据属性
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//发送数据
	m_AndroidUserManager.SendDataToClient(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_COMPUTER);

	return true;
}

//房间消息
bool CAttemperEngineSink::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//变量定义
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//构造数据
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//变量定义
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//数据属性
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//发送数据
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);

			//关闭处理
			if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0) m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//游戏消息
bool CAttemperEngineSink::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if ((pIServerUserItem->GetBindIndex()!=INVALID_WORD)&&(pIServerUserItem->IsClientReady()==true))
	{
		//变量定义
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//构造数据
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//变量定义
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//数据属性
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//发送数据
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);

			//关闭处理
			if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0) m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//房间消息
bool CAttemperEngineSink::SendRoomMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wType, bool bAndroid)
{
	//变量定义
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//构造数据
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//数据属性
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//发送数据
	if (bAndroid)
	{
		//机器用户
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	}
	else
	{
		//常规用户
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	}

	return true;
}

//发送数据
bool CAttemperEngineSink::SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//机器数据
	if ((cbSendMask&BG_COMPUTER)!=0)
	{
		m_AndroidUserManager.SendDataToClient(wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	//用户数据
	m_pITCPNetworkEngine->SendDataBatch(wMainCmdID,wSubCmdID,pData,wDataSize,cbSendMask);

	return true;
}

//发送数据
bool CAttemperEngineSink::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//发送数据
	if (LOWORD(dwSocketID)!=INVALID_WORD)
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			//机器用户
			m_AndroidUserManager.SendDataToClient(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else 
		{
			//网络用户
			m_pITCPNetworkEngine->SendData(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//发送数据
bool CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}

		return true;
	}

	return false;
}

//用户积分
bool CAttemperEngineSink::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_UserScore UserScore;
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	//构造数据
	UserScore.dwUserID=pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	UserScore.UserScore.dwUserMedal=pUserInfo->dwUserMedal;
	UserScore.UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	//构造积分
	UserScore.UserScore.lGrade=pUserInfo->lGrade;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;

	//构造积分
	UserScore.UserScore.lScore=pUserInfo->lScore;
	UserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	UserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//发送数据
	SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));

	//变量定义
	CMD_GR_MobileUserScore MobileUserScore;

	//构造数据
	MobileUserScore.dwUserID=pUserInfo->dwUserID;
	MobileUserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	MobileUserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	MobileUserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	MobileUserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	MobileUserScore.UserScore.dwExperience=pUserInfo->dwExperience;

	//构造积分
	MobileUserScore.UserScore.lScore=pUserInfo->lScore;
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//发送数据
	SendDataBatchToMobileUser(pIServerUserItem->GetTableID(),MDM_GR_USER,SUB_GR_USER_SCORE,&MobileUserScore,sizeof(MobileUserScore));

	//即时写分
	if ((CServerRule::IsImmediateWriteScore(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->IsVariation()==true))
	{
		//变量定义
		DBR_GR_WriteGameScore WriteGameScore;
		ZeroMemory(&WriteGameScore,sizeof(WriteGameScore));

		//用户信息
		WriteGameScore.dwUserID=pIServerUserItem->GetUserID();
		WriteGameScore.dwDBQuestID=pIServerUserItem->GetDBQuestID();
		WriteGameScore.dwClientAddr=pIServerUserItem->GetClientAddr();
		WriteGameScore.dwInoutIndex=pIServerUserItem->GetInoutIndex();

		//提取积分
		pIServerUserItem->DistillVariation(WriteGameScore.VariationInfo);

		//投递请求
		m_pIDBCorrespondManager->PostDataBaseRequest(WriteGameScore.dwUserID,DBR_GR_WRITE_GAME_SCORE,0L,&WriteGameScore,sizeof(WriteGameScore), TRUE);
	}

	//通知桌子
	if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		m_TableFrameArray[pIServerUserItem->GetTableID()]->OnUserScroeNotify(pIServerUserItem->GetChairID(),pIServerUserItem,cbReason);
	}
	return true;
}

//用户状态
bool CAttemperEngineSink::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID)
{


	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_UserStatus UserStatus;
	ZeroMemory(&UserStatus,sizeof(UserStatus));

	//构造数据
	UserStatus.dwUserID=pIServerUserItem->GetUserID();
	UserStatus.UserStatus.wTableID=pIServerUserItem->GetTableID();
	UserStatus.UserStatus.wChairID=pIServerUserItem->GetChairID();
	UserStatus.UserStatus.cbUserStatus=pIServerUserItem->GetUserStatus();

	//修改信息
	if(pIServerUserItem->GetUserStatus()==US_SIT && pIServerUserItem->IsMobileUser())
	{
		//定义变量
		WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
		WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();

		if(pIServerUserItem->GetTableID() > (wTagerDeskPos+wTagerDeskCount-1))
		{
			//定义变量
			WORD wNewDeskPos = (pIServerUserItem->GetTableID()/wTagerDeskCount)*wTagerDeskCount;
			WORD wMaxDeskPos = m_pGameServiceOption->wTableCount-wTagerDeskCount;

			//数量效验
			if(wNewDeskPos > wMaxDeskPos) wNewDeskPos = wMaxDeskPos;

			//修改信息
			pIServerUserItem->SetMobileUserDeskPos(wNewDeskPos);
		}
	}

	//发送数据
	SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	if(pIServerUserItem->GetUserStatus()==US_SIT)
		SendDataBatchToMobileUser(pIServerUserItem->GetTableID(),MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	else
		SendDataBatchToMobileUser(wOldTableID,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));

	//离开判断
	if (pIServerUserItem->GetUserStatus()==US_NULL)
	{
		//获取绑定
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

		//绑带处理
		if (pBindParameter!=NULL)
		{
			//绑定处理
			if (pBindParameter->pIServerUserItem==pIServerUserItem)
			{
				pBindParameter->pIServerUserItem=NULL;
			}

			//中断网络
			if (pBindParameter->dwSocketID!=0L)
			{
				if (LOWORD(pBindParameter->dwSocketID)>=INDEX_ANDROID)
				{
					m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
				}
				else
				{
					printf("ShutDownSocket %d\n", __LINE__);
					m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
				}
			}
		}

		//离开处理
		OnEventUserLogout(pIServerUserItem,0L);
	}
	return true;
}

//用户权限
bool CAttemperEngineSink::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	DBR_GR_ManageUserRight ManageUserRight= {0};
	ManageUserRight.dwUserID = pUserInfo->dwUserID;
	ManageUserRight.dwAddRight = dwAddRight;
	ManageUserRight.dwRemoveRight = dwRemoveRight;
	ManageUserRight.bGameRight=bGameRight;

	//发送请求
	m_pIDBCorrespondManager->PostDataBaseRequest(ManageUserRight.dwUserID,DBR_GR_MANAGE_USER_RIGHT,0,&ManageUserRight,sizeof(ManageUserRight));

	return true;
}

//用户日志 参数:wRequestID 可以作为扩展接口, 暂时没有找到可以放置共有消息号的文件没有实现
bool CAttemperEngineSink::OnEventUserItemLog(IServerUserItem *pIServerUserItem, WORD wRequestID, VOID * pData, DWORD dwDataSize)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return true;

	//小丑操作日志
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_USERLOG_CLOWN_OPERATION,0L,pData,dwDataSize);

	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwContextID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	DBO_GR_LogonSuccess * pDBOLogonSuccess=(DBO_GR_LogonSuccess *)pData;

	//废弃判断
	if ((pBindParameter->pIServerUserItem!=NULL)||(pBindParameter->dwSocketID!=dwContextID))
	{
		//错误断言
		ASSERT(FALSE);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_NORMAL);

		return true;
	}

	//变量定义
	bool bAndroidUser=(wBindIndex>=INDEX_ANDROID);
	bool bMobileUser=(pBindParameter->cbClientKind==CLIENT_KIND_MOBILE);

	//被踢判断
	DWORD dwKickTime;
	if(m_KickUserItemMap.Lookup(pDBOLogonSuccess->dwUserID,dwKickTime)==TRUE)
	{
		//时效判断
		DWORD dwCurrTime = (DWORD)time(NULL);
		if(dwKickTime+VALID_TIME_KICK_BY_MANAGER > dwCurrTime)
		{
			//发送失败
			SendLogonFailure(TEXT("您已被管理员请出房间,1小时之内不能进入！"),0,pBindParameter->dwSocketID);

			//解除锁定
			PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

			return true;
		}
		else
		{
			//移除玩家
			m_KickUserItemMap.RemoveKey(pDBOLogonSuccess->dwUserID);
		}
	}

	//切换判断
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		//切换用户
		SwitchUserItemConnect(pIServerUserItem,pDBOLogonSuccess->szMachineID,wBindIndex,pDBOLogonSuccess->cbDeviceType,pDBOLogonSuccess->wBehaviorFlags,pDBOLogonSuccess->wPageTableCount);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_USER_IMPACT);

		return true;
	}

	//维护判断
	if ((CServerRule::IsForfendRoomEnter(m_pGameServiceOption->dwServerRule)==true)&&(pDBOLogonSuccess->cbMasterOrder==0))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，由于系统维护的原因，当前游戏房间禁止用户进入！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SYSTEM);

		return true;
	}

	//查找机器
	if (bAndroidUser==true)
	{
		//查找机器
		DWORD dwUserID=pDBOLogonSuccess->dwUserID;
		IAndroidUserItem * pIAndroidUserItem=m_AndroidUserManager.SearchAndroidUserItem(dwUserID,dwContextID);

		//修改积分
		if (pIAndroidUserItem!=NULL)
		{
			//获取配置
			tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();

			//调整积分
			if ((pAndroidParameter->lMinTakeScore!=0L)&&(pAndroidParameter->lMaxTakeScore!=0L))
			{
				//变量定义
				SCORE lMinTakeScore=(SCORE)pAndroidParameter->lMinTakeScore;
				SCORE lMaxTakeScore=(SCORE)__max(pAndroidParameter->lMaxTakeScore,pAndroidParameter->lMinTakeScore);

				//调整积分
				if ((lMaxTakeScore-lMinTakeScore)>0L)
				{
					SCORE lTakeScore = (lMaxTakeScore-lMinTakeScore)/10;
					pDBOLogonSuccess->lScore=(SCORE)(lMinTakeScore+(rand()%10)*lTakeScore+rand()%lTakeScore);
				}
				else
				{
					pDBOLogonSuccess->lScore=(SCORE)lMaxTakeScore;
				}
			}
		}
	}

	//最低分数
	//if (m_pGameServiceOption->wKindID==2008)
	//{
	//	if ((m_pGameServiceOption->lMinEnterScore!=0L)&&(pDBOLogonSuccess->lScore<m_pGameServiceOption->lMinEnterScore)&&(pDBOLogonSuccess->nTreasureMapCount<=0))
	//	{
	//		TCHAR szMsg[128]=TEXT("");
	//		_sntprintf(szMsg,CountArray(szMsg), TEXT("您当前的金币不足%I64d，海盗不接受您的挑战，去新手房多捕几条鱼再来挑战吧"), m_pGameServiceOption->lMinEnterScore);
	//		SendLogonFailure(szMsg,0,pBindParameter->dwSocketID);
	//		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);
	//		return true;
	//	}
	//}
	if ((m_pGameServiceOption->lMinEnterScore!=0L)&&(pDBOLogonSuccess->lScore<m_pGameServiceOption->lMinEnterScore))
	{
		//发送失败
		TCHAR szMsg[128]=TEXT("");
		_sntprintf(szMsg,CountArray(szMsg), TEXT("您的金币数低于当前游戏房间的最低进入金币%I64d，不能进入当前游戏房间！"), m_pGameServiceOption->lMinEnterScore);
		SendLogonFailure(szMsg,0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//最高分数
	if ((m_pGameServiceOption->lMaxEnterScore!=0L)&&(pDBOLogonSuccess->lScore>m_pGameServiceOption->lMaxEnterScore))
	{
		//发送失败
		TCHAR szMsg[128]=TEXT("");
		_sntprintf(szMsg,CountArray(szMsg), TEXT("抱歉，您的游戏成绩高于当前游戏房间的最高进入成绩%I64d，不能进入当前游戏房间！"), m_pGameServiceOption->lMaxEnterScore);
		SendLogonFailure(szMsg,0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//会员判断
	if(m_pGameServiceOption->cbMinEnterMember != 0 && pDBOLogonSuccess->cbMemberOrder < m_pGameServiceOption->cbMinEnterMember &&(pDBOLogonSuccess->cbMasterOrder==0))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，您的会员级别低于当前游戏房间的最低进入会员条件，不能进入当前游戏房间！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//会员判断
	if(m_pGameServiceOption->cbMaxEnterMember != 0 && pDBOLogonSuccess->cbMemberOrder > m_pGameServiceOption->cbMaxEnterMember &&(pDBOLogonSuccess->cbMasterOrder==0))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，您的会员级别高于当前游戏房间的最高进入会员条件，不能进入当前游戏房间！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//满人判断
	WORD wMaxPlayer=m_pGameServiceOption->wMaxPlayer;
	DWORD dwOnlineCount=m_ServerUserManager.GetUserItemCount();
	if ((pDBOLogonSuccess->cbMemberOrder==0)&&(pDBOLogonSuccess->cbMasterOrder==0)&&(dwOnlineCount>(DWORD)(wMaxPlayer-RESERVE_USER_COUNT)))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，由于此房间已经人满，普通玩家不能继续进入了！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		return true;
	}

	//用户变量
	tagUserInfo UserInfo;
	tagUserInfoPlus UserInfoPlus;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&UserInfoPlus,sizeof(UserInfoPlus));

	//属性资料
	UserInfo.wFaceID=pDBOLogonSuccess->wFaceID;
	UserInfo.dwUserID=pDBOLogonSuccess->dwUserID;
	UserInfo.dwGameID=pDBOLogonSuccess->dwGameID;
	UserInfo.dwGroupID=pDBOLogonSuccess->dwGroupID;
	UserInfo.dwCustomID=pDBOLogonSuccess->dwCustomID;
	lstrcpyn(UserInfo.szNickName,pDBOLogonSuccess->szNickName,CountArray(UserInfo.szNickName));

	//用户资料
	UserInfo.cbGender=pDBOLogonSuccess->cbGender;
	UserInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
	UserInfo.cbMasterOrder=pDBOLogonSuccess->cbMasterOrder;
	lstrcpyn(UserInfo.szGroupName,pDBOLogonSuccess->szGroupName,CountArray(UserInfo.szGroupName));
	lstrcpyn(UserInfo.szUnderWrite,pDBOLogonSuccess->szUnderWrite,CountArray(UserInfo.szUnderWrite));

	//状态设置
	UserInfo.cbUserStatus=US_FREE;
	UserInfo.wTableID=INVALID_TABLE;
	UserInfo.wChairID=INVALID_CHAIR;

	//积分信息
	UserInfo.lScore=pDBOLogonSuccess->lScore;
	UserInfo.lGrade=pDBOLogonSuccess->lGrade;
	UserInfo.lInsure=pDBOLogonSuccess->lInsure;
	UserInfo.dwWinCount=pDBOLogonSuccess->dwWinCount;
	UserInfo.dwLostCount=pDBOLogonSuccess->dwLostCount;
	UserInfo.dwDrawCount=pDBOLogonSuccess->dwDrawCount;
	UserInfo.dwFleeCount=pDBOLogonSuccess->dwFleeCount;
	UserInfo.dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	UserInfo.dwExperience=pDBOLogonSuccess->dwExperience;
	UserInfo.lLoveLiness=pDBOLogonSuccess->lLoveLiness;

	//登录信息
	UserInfoPlus.dwLogonTime=(DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex=pDBOLogonSuccess->dwInoutIndex;

	//用户权限
	UserInfoPlus.dwUserRight=pDBOLogonSuccess->dwUserRight;
	UserInfoPlus.dwMasterRight=pDBOLogonSuccess->dwMasterRight;

	//辅助变量
	UserInfoPlus.bMobileUser=bMobileUser;
	UserInfoPlus.bAndroidUser=bAndroidUser;
	UserInfoPlus.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	lstrcpyn(UserInfoPlus.szPassword,pDBOLogonSuccess->szPassword,CountArray(UserInfoPlus.szPassword));

	//连接信息
	UserInfoPlus.wBindIndex=wBindIndex;
	UserInfoPlus.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(UserInfoPlus.szMachineID,pDBOLogonSuccess->szMachineID,CountArray(UserInfoPlus.szMachineID));

	//激活用户
	m_ServerUserManager.InsertUserItem(&pIServerUserItem,UserInfo,UserInfoPlus);

	//错误判断
	if (pIServerUserItem==NULL)
	{
		//错误断言
		ASSERT(FALSE);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		//断开用户
		if (bAndroidUser==true)
		{
			m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
		}
		else
		{
			printf("ShutDownSocket %d\n", __LINE__);
			m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
		}

		return true;
	}

	//设置用户
	pBindParameter->pIServerUserItem=pIServerUserItem;

	pIServerUserItem->SetVipInfo(pDBOLogonSuccess->cbVIPLevel, pDBOLogonSuccess->dwVIPExp);

	//修改参数
	if(pIServerUserItem->IsMobileUser())
	{
		SetMobileUserParameter(pIServerUserItem,pDBOLogonSuccess->cbDeviceType,pDBOLogonSuccess->wBehaviorFlags,pDBOLogonSuccess->wPageTableCount);
	}

	//登录事件
	OnEventUserLogon(pIServerUserItem,false);

	//汇总用户
	if (m_bCollectUser==true)
	{
		//变量定义
		CMD_CS_C_UserEnter UserEnter;
		ZeroMemory(&UserEnter,sizeof(UserEnter));

		//设置变量
		UserEnter.dwUserID=pIServerUserItem->GetUserID();
		UserEnter.dwGameID=pIServerUserItem->GetGameID();
		lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

		//辅助信息
		UserEnter.cbGender=pIServerUserItem->GetGender();
		UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
		UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();
		UserEnter.cbAndroid=pIServerUserItem->IsAndroidUser();

		//发送消息
		ASSERT(m_pITCPSocketService!=NULL);
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));
	}

	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem!=NULL)) return true;

	//发送错误
	DBO_GR_LogonFailure * pLogonFailure=(DBO_GR_LogonFailure *)pData;
	SendLogonFailure(pLogonFailure->szDescribeString,pLogonFailure->lResultCode,dwContextID);

	//断开连接
	if (LOWORD(dwContextID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
	}
	else
	{
		printf("ShutDownSocket %d\n", __LINE__);
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}

	return true;
}

//配置信息
bool CAttemperEngineSink::OnDBGameParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBO_GR_GameParameter));
	if (wDataSize!=sizeof(DBO_GR_GameParameter)) return false;

	//变量定义
	DBO_GR_GameParameter * pGameParameter=(DBO_GR_GameParameter *)pData;

	//汇率信息
	m_pGameParameter->wMedalRate=pGameParameter->wMedalRate;
	m_pGameParameter->wRevenueRate=pGameParameter->wRevenueRate;

	//版本信息
	m_pGameParameter->dwClientVersion=pGameParameter->dwClientVersion;
	m_pGameParameter->dwServerVersion=pGameParameter->dwServerVersion;

	//版本效验
	if (VERSION_EFFICACY==TRUE)
	{
		//版本判断
		bool bVersionInvalid=false;
		if (m_pGameParameter->dwClientVersion!=m_pGameServiceAttrib->dwClientVersion) bVersionInvalid=true;
		if (m_pGameParameter->dwServerVersion!=m_pGameServiceAttrib->dwServerVersion) bVersionInvalid=true;

		//提示信息
		if (bVersionInvalid==true)
		{
			CTraceService::TraceString(TEXT("平台数据库服务组件版本注册信息与当前组件版本信息不一致"),TraceLevel_Warning);
		}
	}

	return true;
}

//列表信息
bool CAttemperEngineSink::OnDBGameColumnInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_GameColumnInfo * pGameColumnInfo=(DBO_GR_GameColumnInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameColumnInfo)-sizeof(pGameColumnInfo->ColumnItemInfo);

	//效验参数
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0])))) return false;

	//数据处理
	if (pGameColumnInfo->cbColumnCount==0)
	{
		//默认列表
	}
	else
	{
		//拷贝数据
		m_DataConfigColumn.cbColumnCount=pGameColumnInfo->cbColumnCount;
		CopyMemory(m_DataConfigColumn.ColumnItem,pGameColumnInfo->ColumnItemInfo,pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]));
	}

	return true;
}

//机器信息
bool CAttemperEngineSink::OnDBGameAndroidInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_GameAndroidInfo * pGameAndroidInfo=(DBO_GR_GameAndroidInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameAndroidInfo)-sizeof(pGameAndroidInfo->AndroidParameter);

	//效验参数
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0])))) return false;

	//设置机器
	if (pGameAndroidInfo->lResultCode==DB_SUCCESS)
	{
		m_AndroidUserManager.SetAndroidStock(pGameAndroidInfo->AndroidParameter,pGameAndroidInfo->wAndroidCount);
	}

	return true;
}

//道具信息
bool CAttemperEngineSink::OnDBGamePropertyInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_GamePropertyInfo * pGamePropertyInfo=(DBO_GR_GamePropertyInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GamePropertyInfo)-sizeof(pGamePropertyInfo->PropertyInfo);

	//效验参数
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0])))) return false;

	//获取状态
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	enServiceStatus ServiceStatus=pServiceUnits->GetServiceStatus();

	//设置道具
	if (pGamePropertyInfo->lResultCode==DB_SUCCESS)
	{
		//设置管理
		m_GamePropertyManager.SetGamePropertyInfo(pGamePropertyInfo->PropertyInfo,pGamePropertyInfo->cbPropertyCount);

		//拷贝数据
		m_DataConfigProperty.cbPropertyCount=pGamePropertyInfo->cbPropertyCount;
		CopyMemory(m_DataConfigProperty.PropertyInfo,pGamePropertyInfo->PropertyInfo,pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0]));
	}

	//事件通知
	if (ServiceStatus!=ServiceStatus_Service)
	{
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_SERVICE_CONFIG_RESULT,&ControlResult,sizeof(ControlResult));
	}

	return true;
}

//银行信息
bool CAttemperEngineSink::OnDBUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//变量定义
	DBO_GR_UserInsureInfo * pUserInsureInfo=(DBO_GR_UserInsureInfo *)pData;

	//变量定义
	CMD_GR_S_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//构造数据
	UserInsureInfo.cbActivityGame=pUserInsureInfo->cbActivityGame;
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lUserScore+=pIServerUserItem->GetUserScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetTrusteeScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetFrozenedScore();
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GR_INSURE,SUB_GR_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	return true;
}

//赠送信息
bool CAttemperEngineSink::OnDBUserInsureTransferInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_TRANSFER_INFO,pData,wDataSize);

	return true;
}

//银行成功
bool CAttemperEngineSink::OnDBUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//变量定义
	DBO_GR_UserInsureSuccess * pUserInsureSuccess=(DBO_GR_UserInsureSuccess *)pData;

	//变量定义
	SCORE lFrozenedScore=pUserInsureSuccess->lFrozenedScore;
	SCORE lInsureRevenue=pUserInsureSuccess->lInsureRevenue;
	SCORE lVariationScore=pUserInsureSuccess->lVariationScore;
	SCORE lVariationInsure=pUserInsureSuccess->lVariationInsure;

	//解冻积分
	if ((lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(lFrozenedScore)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//银行操作
	if (pIServerUserItem->ModifyUserInsure(lVariationScore,lVariationInsure,lInsureRevenue)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	CMD_GR_S_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//构造变量
	UserInsureSuccess.cbActivityGame=pUserInsureSuccess->cbActivityGame;
	UserInsureSuccess.lUserScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//操作失败
bool CAttemperEngineSink::OnDBUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	WORD wBindIndex=LOWORD(dwContextID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送错误
	DBO_GR_UserInsureFailure * pUserInsureFailure=(DBO_GR_UserInsureFailure *)pData;
	SendInsureFailure(pIServerUserItem,pUserInsureFailure->szDescribeString,pUserInsureFailure->lResultCode,pUserInsureFailure->cbActivityGame);

	//解冻积分
	if ((pUserInsureFailure->lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(pUserInsureFailure->lFrozenedScore)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//用户信息
bool CAttemperEngineSink::OnDBUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//变量定义
	DBO_GR_UserTransferUserInfo * pTransferUserInfo=(DBO_GR_UserTransferUserInfo *)pData;

	//变量定义
	CMD_GR_S_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//构造变量
	UserTransferUserInfo.cbActivityGame=pTransferUserInfo->cbActivityGame;
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szNickName,pTransferUserInfo->szNickName,CountArray(UserTransferUserInfo.szNickName));

	//发送数据
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_TRANSFER_USER_INFO,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//道具成功
bool CAttemperEngineSink::OnDBPropertySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_S_PropertySuccess * pPropertySuccess=(DBO_GR_S_PropertySuccess *)pData;

	//获取用户
	IServerUserItem * pISourceUserItem=m_ServerUserManager.SearchUserItem(pPropertySuccess->dwSourceUserID);
	IServerUserItem * pITargetUserItem=m_ServerUserManager.SearchUserItem(pPropertySuccess->dwTargetUserID);

	//赠送用户
	if (pISourceUserItem!=NULL)
	{
		//变量定义
		SCORE lFrozenedScore=pPropertySuccess->lFrozenedScore;

		//解冻积分
		if ((lFrozenedScore>0L)&&(pISourceUserItem->UnFrozenedUserScore(lFrozenedScore)==false))
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//更新魅力
	if (pISourceUserItem!=NULL)
	{
		pISourceUserItem->ModifyUserProperty(0,pPropertySuccess->lSendLoveLiness);
	}

	//更新魅力
	if(pITargetUserItem!=NULL)
	{
		pITargetUserItem->ModifyUserProperty(0,pPropertySuccess->lRecvLoveLiness);
	}

	//变量定义
	DWORD dwCurrentTime=(DWORD)time(NULL);
	tagUserProperty * pUserProperty = pITargetUserItem->GetUserProperty();

	//道具处理	
	switch(pPropertySuccess->wPropertyIndex)
	{
	case PROPERTY_ID_TWO_CARD:       //双倍积分
		{
			//使用判断
			if((pUserProperty->wPropertyUseMark&PT_USE_MARK_DOUBLE_SCORE)!=0)
			{
				//变量定义
				DWORD  dwValidTime=pUserProperty->PropertyInfo[0].wPropertyCount*pUserProperty->PropertyInfo[0].dwValidNum;
				if(pUserProperty->PropertyInfo[0].dwEffectTime+dwValidTime<dwCurrentTime)
				{
					pUserProperty->PropertyInfo[0].dwEffectTime=dwCurrentTime;
					pUserProperty->PropertyInfo[0].wPropertyCount=pPropertySuccess->wItemCount;
					pUserProperty->PropertyInfo[0].dwValidNum=VALID_TIME_DOUBLE_SCORE;
				}
				else
				{
					//数目累加
					pUserProperty->PropertyInfo[0].wPropertyCount+=pPropertySuccess->wItemCount;
				}
			}
			else
			{
				//设置信息
				pUserProperty->PropertyInfo[0].dwEffectTime=dwCurrentTime;
				pUserProperty->PropertyInfo[0].wPropertyCount=pPropertySuccess->wItemCount;
				pUserProperty->PropertyInfo[0].dwValidNum=VALID_TIME_DOUBLE_SCORE;
				pUserProperty->wPropertyUseMark |= PT_USE_MARK_DOUBLE_SCORE;
			}

			break;
		}
	case PROPERTY_ID_FOUR_CARD:      //四倍积分
		{
			//使用判断
			if((pUserProperty->wPropertyUseMark&PT_USE_MARK_FOURE_SCORE)!=0)
			{
				//变量定义
				DWORD  dwValidTime=pUserProperty->PropertyInfo[1].wPropertyCount*pUserProperty->PropertyInfo[1].dwValidNum;
				if(pUserProperty->PropertyInfo[1].dwEffectTime+dwValidTime<dwCurrentTime)
				{
					pUserProperty->PropertyInfo[1].dwEffectTime=dwCurrentTime;
					pUserProperty->PropertyInfo[1].wPropertyCount=pPropertySuccess->wItemCount;
					pUserProperty->PropertyInfo[1].dwValidNum=VALID_TIME_FOUR_SCORE;
				}
				else
				{
					//数目累加
					pUserProperty->PropertyInfo[1].wPropertyCount+=pPropertySuccess->wItemCount;
				}
			}
			else
			{
				//设置信息
				pUserProperty->PropertyInfo[1].dwEffectTime=dwCurrentTime;
				pUserProperty->PropertyInfo[1].wPropertyCount=pPropertySuccess->wItemCount;
				pUserProperty->PropertyInfo[1].dwValidNum=VALID_TIME_FOUR_SCORE;
				pUserProperty->wPropertyUseMark |= PT_USE_MARK_FOURE_SCORE;
			}
			break;
		}
	case PROPERTY_ID_SCORE_CLEAR:    //负分清零
		{
			//变量定义
			SCORE lCurrScore = pITargetUserItem->GetUserScore();
			if ( lCurrScore < 0)
			{
				//用户信息
				tagUserInfo * pUserInfo = pITargetUserItem->GetUserInfo();
				if(pUserInfo==NULL) return true;

				//修改积分
				pUserInfo->lScore=0;
			}
			break;
		}
	case PROPERTY_ID_ESCAPE_CLEAR:   //逃跑清零
		{
			//用户信息
			tagUserInfo * pUserInfo = pITargetUserItem->GetUserInfo();
			if(pUserInfo==NULL) return true;

			//修改逃跑率
			if(pUserInfo->dwFleeCount > 0)
			{
				pUserInfo->dwFleeCount=0;
			}

			break;
		}
	case PROPERTY_ID_GUARDKICK_CARD: //防踢卡
		{
			//使用判断
			if((pUserProperty->wPropertyUseMark&PT_USE_MARK_GUARDKICK_CARD)!=0)
			{
				//变量定义
				DWORD  dwValidTime=pUserProperty->PropertyInfo[2].wPropertyCount*pUserProperty->PropertyInfo[2].dwValidNum;
				if(pUserProperty->PropertyInfo[2].dwEffectTime+dwValidTime<dwCurrentTime)
				{
					pUserProperty->PropertyInfo[2].dwEffectTime=dwCurrentTime;
					pUserProperty->PropertyInfo[2].wPropertyCount=pPropertySuccess->wItemCount;
					pUserProperty->PropertyInfo[2].dwValidNum=VALID_TIME_GUARDKICK_CARD;
				}
				else
				{
					//数目累加
					pUserProperty->PropertyInfo[2].wPropertyCount+=pPropertySuccess->wItemCount;
				}
			}
			else
			{
				//设置信息
				pUserProperty->PropertyInfo[2].dwEffectTime=dwCurrentTime;
				pUserProperty->PropertyInfo[2].wPropertyCount=pPropertySuccess->wItemCount;
				pUserProperty->PropertyInfo[2].dwValidNum=VALID_TIME_GUARDKICK_CARD;
				pUserProperty->wPropertyUseMark |= PT_USE_MARK_GUARDKICK_CARD;
			}

			break;
		}
	case PROPERTY_ID_POSSESS:        //附身符
		{
			//使用判断
			if((pUserProperty->wPropertyUseMark&PT_USE_MARK_POSSESS)!=0)
			{
				//变量定义
				DWORD  dwValidTime=pUserProperty->PropertyInfo[3].wPropertyCount*pUserProperty->PropertyInfo[3].dwValidNum;
				if(pUserProperty->PropertyInfo[3].dwEffectTime+dwValidTime<dwCurrentTime)
				{
					pUserProperty->PropertyInfo[3].dwEffectTime=dwCurrentTime;
					pUserProperty->PropertyInfo[3].wPropertyCount=pPropertySuccess->wItemCount;
					pUserProperty->PropertyInfo[3].dwValidNum=VALID_TIME_POSSESS;
				}
				else
				{
					//数目累加
					pUserProperty->PropertyInfo[3].wPropertyCount+=pPropertySuccess->wItemCount;
				}
			}
			else
			{
				//设置信息
				pUserProperty->PropertyInfo[3].dwEffectTime=dwCurrentTime;
				pUserProperty->PropertyInfo[3].wPropertyCount=pPropertySuccess->wItemCount;
				pUserProperty->PropertyInfo[3].dwValidNum=VALID_TIME_POSSESS;
				pUserProperty->wPropertyUseMark |= PT_USE_MARK_POSSESS;
			}

			break;
		}
	case PROPERTY_ID_BLUERING_CARD:  //蓝钻会员
	case PROPERTY_ID_YELLOWRING_CARD://黄钻会员
	case PROPERTY_ID_WHITERING_CARD: //白钻会员
	case PROPERTY_ID_REDRING_CARD:   //红钻会员
	case PROPERTY_ID_VIPROOM_CARD:   //VIP会员
		{
			//用户信息
			tagUserInfo * pUserInfo = pITargetUserItem->GetUserInfo();
			if(pUserInfo==NULL) return true;

			//更新会员
			pUserInfo->cbMemberOrder=pPropertySuccess->cbMemberOrder;
			
			//修改权限
			pITargetUserItem->ModifyUserRight(pPropertySuccess->dwUserRight,0);

			//发送消息
			SendPropertyEffect(pITargetUserItem);

			break;
		}
	default:  // 全部礼物
		{			
			break;
		}
	}

	//消费方式
	if(pPropertySuccess->cbConsumeScore==FALSE)
	{
		pISourceUserItem->ModifyUserInsure(0,-pPropertySuccess->lConsumeGold,0);
	}

	//喇叭判断
	if(pPropertySuccess->wPropertyIndex==PROPERTY_ID_TRUMPET|| pPropertySuccess->wPropertyIndex==PROPERTY_ID_TYPHON)
		return true;

	//变量定义
	CMD_GR_S_PropertySuccess PropertySuccess;
	ZeroMemory(&PropertySuccess,sizeof(PropertySuccess));

	//设置变量
	PropertySuccess.cbRequestArea=pPropertySuccess->cbRequestArea;
	PropertySuccess.wItemCount=pPropertySuccess->wItemCount;
	PropertySuccess.wPropertyIndex=pPropertySuccess->wPropertyIndex;
	PropertySuccess.dwSourceUserID=pPropertySuccess->dwSourceUserID;
	PropertySuccess.dwTargetUserID=pPropertySuccess->dwTargetUserID;

	//发送消息
	if (pISourceUserItem!=NULL)
	{
		SendData(pISourceUserItem,MDM_GR_USER,SUB_GR_PROPERTY_SUCCESS,&PropertySuccess,sizeof(PropertySuccess));
	}

	//发送消息
	if (pITargetUserItem!=NULL && pITargetUserItem!=pISourceUserItem)
	{
		SendData(pITargetUserItem,MDM_GR_USER,SUB_GR_PROPERTY_SUCCESS,&PropertySuccess,sizeof(PropertySuccess));
	}

	//广播礼物
	if(GetPropertyType(pPropertySuccess->wPropertyIndex)==PT_TYPE_PRESENT)
		SendPropertyMessage(PropertySuccess.dwSourceUserID,PropertySuccess.dwTargetUserID,PropertySuccess.wPropertyIndex,
		PropertySuccess.wItemCount);

	return true;
}

//道具失败
bool CAttemperEngineSink::OnDBPropertyFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize==sizeof(DBO_GR_PropertyFailure));
	if(wDataSize!=sizeof(DBO_GR_PropertyFailure)) return false;

	//提取数据
	DBO_GR_PropertyFailure * pPropertyFailure = (DBO_GR_PropertyFailure *)pData;
	if(pPropertyFailure==NULL) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwContextID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	if(pBindParameter==NULL) return false;
	if(pBindParameter->pIServerUserItem==NULL) return false;

	//发送消息
	return SendPropertyFailure(pBindParameter->pIServerUserItem,pPropertyFailure->szDescribeString,0L,pPropertyFailure->cbRequestArea);
}

//系统消息
bool CAttemperEngineSink::OnDBSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//参数校验
	ASSERT(wDataSize==sizeof(DBR_GR_SystemMessage));
	if(wDataSize!=sizeof(DBR_GR_SystemMessage)) return false;

	//提取数据
	DBR_GR_SystemMessage * pSystemMessage = (DBR_GR_SystemMessage *)pData;
	if(pSystemMessage==NULL) return false;

	//重复判断
	POSITION pos = m_SystemMessageList.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION tempPos = pos;
		tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);
		if(pTagSystemMessage->SystemMessage.cbMessageID == pSystemMessage->cbMessageID)
		{
			//更新数据
			pTagSystemMessage->dwLastTime=0;
			CopyMemory(&pTagSystemMessage->SystemMessage, pSystemMessage, sizeof(DBR_GR_SystemMessage));

			return true;
		}
	}

	//定于变量
	tagSystemMessage  *pSendMessage=new tagSystemMessage;
	ZeroMemory(pSendMessage, sizeof(tagSystemMessage));

	//设置变量
	CopyMemory(&pSendMessage->SystemMessage,pSystemMessage, sizeof(DBR_GR_SystemMessage));

	//记录消息
	m_SystemMessageList.AddTail(pSendMessage);

	return true;
}

//加载敏感词
bool CAttemperEngineSink::OnDBSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//开始加载
	if(dwContextID==0xfffe)
	{
		m_WordsFilter.ResetSensitiveWordArray();
		m_pITimerEngine->KillTimer(IDI_LOAD_SENSITIVE_WORD);
		return true;			
	}

	//加载完成
	if(dwContextID==0xffff)
	{
		m_WordsFilter.FinishAdd();
		return true;
	}

	//加载敏感词
	const TCHAR *pWords=(const TCHAR*)pData;
	m_WordsFilter.AddSensitiveWords(pWords);
	return true;
}

//注册事件
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//注册失败
		{
			//变量定义
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//效验参数
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//关闭处理
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//显示消息
			if (lstrlen(pRegisterFailure->szDescribeString)>0)
			{
				CTraceService::TraceString(pRegisterFailure->szDescribeString,TraceLevel_Exception);
			}

			//事件通知
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//列表事件
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//房间信息
		{
			//废弃列表
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//房间人数
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//变量定义
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);

			//设置人数
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//房间插入
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//房间修改
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//变量定义
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//设置房间
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//房间删除
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//变量定义
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//变量定义
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//房间完成
		{
			//清理列表
			m_ServerListManager.CleanServerItem();

			//事件处理
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//汇总事件
bool CAttemperEngineSink::OnTCPSocketMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_COLLECT_REQUEST:	//用户汇总
		{
			//变量定义
			CMD_CS_C_UserEnter UserEnter;
			ZeroMemory(&UserEnter,sizeof(UserEnter));

			//发送用户
			WORD wIndex=0;
			do
			{
				//获取用户
				IServerUserItem * pIServerUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
				if (pIServerUserItem==NULL) break;

				//设置变量
				UserEnter.dwUserID=pIServerUserItem->GetUserID();
				UserEnter.dwGameID=pIServerUserItem->GetGameID();
				lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

				//辅助信息
				UserEnter.cbGender=pIServerUserItem->GetGender();
				UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
				UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();
				UserEnter.cbAndroid=pIServerUserItem->IsAndroidUser();

				//发送数据
				ASSERT(m_pITCPSocketService!=NULL);
				m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));

			} while (true);

			//汇报完成
			m_bCollectUser=true;
			m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_FINISH);

			return true;
		}
	}

	return true;
}

//管理服务
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SYSTEM_MESSAGE:	//系统消息
		{
			//消息处理
			SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);

			return true;
		}
	case SUB_CS_S_PROPERTY_TRUMPET:  //喇叭消息
		{
			//发送数据
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_PROPERTY_TRUMPET,pData,wDataSize,BG_COMPUTER);

			return true;
		}
	case SUB_CS_S_VIP_TRUMPET:
		{
			//发送数据
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_S_VIP_TRUMPET,pData,wDataSize,BG_COMPUTER);

			return true;
		}
	case SUB_CS_S_VIPUpgrade:
		{
			//printf("SUB_CS_S_VIPUpgrade %s\n", pData);
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_VIP_UPGRADE,pData,wDataSize,BG_COMPUTER);
			return true;
		}
	case SUB_CS_S_LOTTERY_INFO:
		{
			printf("SUB_CS_S_LOTTERY_INFO\n");
			if (wDataSize != sizeof(CMD_CS_S_LotteryInfo))
			{
				return true;
			}

			CMD_CS_S_LotteryInfo * pLotteryInfo = (CMD_CS_S_LotteryInfo *)pData;

			//更新话费
			m_LotteryInfo.bCouldGet = true;
			m_LotteryInfo.nPrice = pLotteryInfo->nPrice;
			m_LotteryInfo.nMaxGet = pLotteryInfo->nMaxGet;
			m_LotteryInfo.nLessTime = pLotteryInfo->nLessTime;
			m_LotteryInfo.nPercent = pLotteryInfo->nPercent;
			m_LotteryInfo.nMinusStorage = pLotteryInfo->nMinusStorage;

			return true;
		}
	case SUB_CS_S_MOBILE_MONEY:
		{
			printf("SUB_CS_S_MOBILE_MONEY\n");
			if (wDataSize != sizeof(CMD_CS_S_MobileMoney))
			{
				return true;
			}

			CMD_CS_S_MobileMoney * pMobileMoney = (CMD_CS_S_MobileMoney *)pData;

			//更新话费
			m_MobileMoney.bCouldGet = true;
			m_MobileMoney.wPrice = pMobileMoney->wPrice;
			m_MobileMoney.wMaxGet = pMobileMoney->wMaxGet;
			m_MobileMoney.nLessTime = pMobileMoney->nLessTime;
			m_MobileMoney.nPercent = pMobileMoney->nPercent;
			m_MobileMoney.nMinusStorage = pMobileMoney->nMinusStorage;

			return true;
		}
	case SUB_CS_S_MatchRank:
		{
			if (wDataSize != sizeof(CMD_CS_S_MatchRank))
			{
				return true;
			}

			CMD_CS_S_MatchRank * pMatchRank = (CMD_CS_S_MatchRank *)pData;

			IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pMatchRank->dwUserID);
			if (pIServerUserItem != NULL && pIServerUserItem->GetTableID() != INVALID_TABLE)
			{
				m_TableFrameArray[pIServerUserItem->GetTableID()]->OnMatchRank(pIServerUserItem, pMatchRank->lMatchScore, pMatchRank->nCurrentRank, pMatchRank->nChangeRank);
			}

			return true;
		}
	case SUB_CS_S_PEARL_INFO:
		{
			if (wDataSize != sizeof(CMD_CS_S_PearlInfo))
			{
				return true;
			}

			CMD_CS_S_PearlInfo * pPearlInfo = (CMD_CS_S_PearlInfo *)pData;

			IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pPearlInfo->dwUserID);
			if (pIServerUserItem->GetPearl() < 0)
			{
				pIServerUserItem->SetPearl(pPearlInfo->nType);
			}

			return true;
		}
	case SUB_CS_S_BROAD_LABA:
		{
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_BROADLABA,pData,wDataSize,BG_COMPUTER);
			return true;
		}
	case SUB_CS_S_MATCH_SIGNUP_RES:
		{
			if (wDataSize != sizeof(CMD_CS_S_MatchSignUpRes))
			{
				return true;
			}

			// receive from corresponding
			CMD_CS_S_MatchSignUpRes * pMatchSignUpRes = (CMD_CS_S_MatchSignUpRes *) pData;

			//send to User
			CMD_GRO_MatchSignupRes stUserMatchSignUPRes = {0};
			stUserMatchSignUPRes.bMatchResult = false;

			// Get User Info
			IServerUserItem * pIServerUserItem = NULL;
			pIServerUserItem = m_ServerUserManager.SearchUserItem(pMatchSignUpRes->dwUserID);

			// 更新比赛玩家组号
			printf("报名结果\n");
			if (pMatchSignUpRes->bIsSignUpSuc == true)
			{
				tagMatchUserInfoGameServer stMatchUserInfoGameServer = {0};
				stMatchUserInfoGameServer.dwUserID = pMatchSignUpRes->dwUserID;
				stMatchUserInfoGameServer.wGroupID = pMatchSignUpRes->wGroupID;
				stMatchUserInfoGameServer.lUserScore = pIServerUserItem->GetUserScore();
				
				printf("insert info userid = %ld, wgroupid = %d, userscore = %lld\n", pMatchSignUpRes->dwUserID, pMatchSignUpRes->wGroupID, stMatchUserInfoGameServer.lUserScore);

				m_vMatchUserInfo[pMatchSignUpRes->nMatchID - 1].push_back(stMatchUserInfoGameServer);
				printf("用户比赛数据大小=%d,MatchID=%d,MatchIDsize=%d\n",m_vMatchUserInfo.size(),pMatchSignUpRes->nMatchID,m_vMatchUserInfo[pMatchSignUpRes->nMatchID - 1].size());

				stUserMatchSignUPRes.bMatchResult = true;
				stUserMatchSignUPRes.llUserScore = pIServerUserItem->GetUserScore();
				stUserMatchSignUPRes.bMatchStatus = pMatchSignUpRes->bMatchStatus;
				stUserMatchSignUPRes.nMatchID = pMatchSignUpRes->nMatchID;
				lstrcpyn(stUserMatchSignUPRes.szDescription,pMatchSignUpRes->szDescription,sizeof(stUserMatchSignUPRes.szDescription));
			}
			else
			{
				stUserMatchSignUPRes.bMatchResult = false;
				stUserMatchSignUPRes.llUserScore = 0;
				stUserMatchSignUPRes.bMatchStatus = pMatchSignUpRes->bMatchStatus;
				stUserMatchSignUPRes.nMatchID = pMatchSignUpRes->nMatchID;
				lstrcpyn(stUserMatchSignUPRes.szDescription,pMatchSignUpRes->szDescription,sizeof(stUserMatchSignUPRes.szDescription));
			}

			// 给玩家发送报名成功的消息
			SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_MATCH_SIGNUP_RES, &stUserMatchSignUPRes, sizeof(CMD_GRO_MatchSignupRes));

			return true;
		}
	case SUB_CS_S_MATCH_BEGIN:
		{
			printf("收到比赛开始信息\n");
			if (sizeof(CMD_CS_S_MatchBegin) != wDataSize)
			{
				return false;
			}

			// Begin Update UserScore Timer
			m_pITimerEngine->SetTimer(IDI_MATCH_GET_USERSCORE, TIME_MATCH_GET_USERSCORE_INTERVAL, (DWORD)(-1),NULL);

			CMD_CS_S_MatchBegin * pMatchBeginInfo = (CMD_CS_S_MatchBegin *)pData;
			m_cbMatchStatus = MATCH_STATUS_BEGIN;
			printf("比赛开始信息=%d\n",pMatchBeginInfo->nMatchID);
			m_nCurrentMatchID = pMatchBeginInfo->nMatchID;

			IServerUserItem * pIServerUserItem = NULL;
			CMD_GRO_MatchBegin stMatchBegin;	
			ZeroMemory(&stMatchBegin, sizeof(stMatchBegin));
			stMatchBegin.nMatchID = pMatchBeginInfo->nMatchID;
			lstrcpyn(stMatchBegin.szMatchDate,pMatchBeginInfo->szMatchDate,CountArray(stMatchBegin.szMatchDate));
			lstrcpyn(stMatchBegin.szMatchTitle,pMatchBeginInfo->szMatchName,CountArray(stMatchBegin.szMatchTitle));
			
			for (int i = 0; i < m_vMatchUserInfo[m_nCurrentMatchID - 1].size(); i ++)
			{
				pIServerUserItem = m_ServerUserManager.SearchUserItem(m_vMatchUserInfo[m_nCurrentMatchID - 1][i].dwUserID);
				SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_MATCH_BEGIN, &stMatchBegin, sizeof(CMD_GR_Match_Begin_Notify));
			}


			return true;
		}
	case SUB_CS_S_MATCH_END:
		{
			WORD wTempDataSize = 0;
			CMD_CS_S_MatchEndInfo * pMatchInfoNotify = (CMD_CS_S_MatchEndInfo *)pData;
			m_cbMatchStatus = MATCH_STATUS_END;
			wTempDataSize = sizeof(WORD)*2 + pMatchInfoNotify->wVectorSize * sizeof(tagMatchEndInfo);
			printf("wTempDataSize=%d,wDataSize=%d\n",wTempDataSize,wDataSize);
			if (wTempDataSize != wDataSize)
			{
				return false;
			}

			// Kill Update UserScore Timer
			m_pITimerEngine->KillTimer(IDI_MATCH_GET_USERSCORE);

			IServerUserItem * pIServerUserItem = NULL;
			CMD_GRO_MatchEnd stUserMatchEnd={0};	
			tagMatchRewardConfigItem stMatchRewardItem;
			for (int i = 0; i < pMatchInfoNotify->wVectorSize; ++i)
			{
				stUserMatchEnd.nRanking = pMatchInfoNotify->vUserMatchEndInfo[i].nRanking;
				stUserMatchEnd.llScore = pMatchInfoNotify->vUserMatchEndInfo[i].lUserScore;
				GetUserMatchRewardInfo(stMatchRewardItem,stUserMatchEnd.nRanking,pMatchInfoNotify->wMatchType);
				stUserMatchEnd.cbShare = stMatchRewardItem.nShareType;
				pIServerUserItem = m_ServerUserManager.SearchUserItem(pMatchInfoNotify->vUserMatchEndInfo[i].dwUserID);	
				if (pIServerUserItem==NULL)
				{
					continue;
				}
				printf("发送玩家数据玩家名次=%d,玩家分数=%d，是否需要分享=%d\n",stUserMatchEnd.nRanking,stUserMatchEnd.llScore,stUserMatchEnd.cbShare);
				SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_MATCH_END, &stUserMatchEnd, sizeof(CMD_GRO_MatchEnd));
			}

			m_nCurrentMatchID = 0;
			printf("desconstructor m_nCurrentMatchID\n");

			return true;
		}
	case SUB_CS_S_MATCH_SCORE_UPDATE: // GameServer Get the msg from the 
		{
			CMD_CS_S_MatchScoreUpdate * pMatchScoreUpdate = (CMD_CS_S_MatchScoreUpdate *)pData;
			WORD wTmpDataSize = pMatchScoreUpdate->wUserCnt * sizeof(tagUpdateInfoItem) + sizeof(WORD) * 4 + sizeof(int);
			printf(" wTmpDataSize = %ld, wDataSize = %ld \n", wTmpDataSize, wDataSize);
			if (wTmpDataSize != wDataSize)
			{
				return false;
			}

			if (m_nCurrentMatchID == 0)
			{
				printf ("Invalid Current Match ID\n");
				return false;
			}

			IServerUserItem * pIServerUserItem=NULL;
			CMD_GR_MatchScoreUpdate stMatchScoreUpdate = {0};
			WORD wSendDataSize = 0;
			WORD wGroupID = 0;
			bool bIsSendDataPack = false;

			printf("发送分数改变人数=%d\n",m_vMatchUserInfo[m_nCurrentMatchID -1].size());
			for (int i = 0; i < m_vMatchUserInfo[m_nCurrentMatchID -1].size(); i++)			
			{
				wGroupID = m_vMatchUserInfo[m_nCurrentMatchID - 1][i].wGroupID;
				 
				// fill the vector
				stMatchScoreUpdate.nMatchNum = 0;	
				for (int j = 0; j < pMatchScoreUpdate->wUserNumPerGroup; j++)
				{
					if (m_vMatchUserInfo[m_nCurrentMatchID - 1][i].dwUserID 
						== pMatchScoreUpdate->aMatchUserUpdInfo[(wGroupID - 1) * m_wUserCntInGroup + j].dwUserID)
					{
						bIsSendDataPack = true;
						stMatchScoreUpdate.nRanking = j + 1;
						stMatchScoreUpdate.lUserScore = pMatchScoreUpdate->aMatchUserUpdInfo[(wGroupID - 1) * m_wUserCntInGroup + j].lUserScore;
						printf("分数=%lld\n",stMatchScoreUpdate.lUserScore);
					}

					stMatchScoreUpdate.MatchGroupInfo[j].llScore = pMatchScoreUpdate->aMatchUserUpdInfo[(wGroupID - 1) * m_wUserCntInGroup + j].lUserScore;
					lstrcpyn(stMatchScoreUpdate.MatchGroupInfo[j].szNickName,
						pMatchScoreUpdate->aMatchUserUpdInfo[(wGroupID - 1) * m_wUserCntInGroup + j].szNickName,
						LEN_NICKNAME);
					stMatchScoreUpdate.nMatchNum ++;
				}

				if (bIsSendDataPack == false)
				{
					printf("User %ld Data Package failed \n", m_vMatchUserInfo[m_nCurrentMatchID -1][i].dwUserID);	
				}

				stMatchScoreUpdate.nSecond = pMatchScoreUpdate->nSecond;

				wSendDataSize = sizeof(CMD_GR_MatchScoreUpdate) - sizeof(CMD_GRO_MatchGroupInfo) * (100 - stMatchScoreUpdate.nMatchNum);
				pIServerUserItem=m_ServerUserManager.SearchUserItem(m_vMatchUserInfo[m_nCurrentMatchID - 1][i].dwUserID);
				if (pIServerUserItem==NULL)
				{
					return false;
				}
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_MATCH_SCORE_UPDATE,&stMatchScoreUpdate,wSendDataSize);

				wGroupID = 0;
				wSendDataSize = 0;
				pIServerUserItem = NULL;
				bIsSendDataPack = false;
				ZeroMemory(&stMatchScoreUpdate,sizeof(CMD_GR_MatchScoreUpdate));
			}

			return true;
		}
	case SUB_CS_S_MATCH_NOTIFICATION:
		{
			MatchNotification * pNotifyInfo = (MatchNotification *)pData;
			//遍历当前所有玩家，有参赛数据的为已报名
			tagBindParameter * pBindParameter=m_pNormalParameter;
			std::vector<IServerUserItem*> vNoSignUpUser;
			if (m_nCurrentMatchID==0)
			{
				m_nCurrentMatchID=pNotifyInfo->nMatchID;
			}

			printf("收到协调提示消息\n");
			printf("当前游戏场次=%d\n",m_nCurrentMatchID);
			printf("当前场次人数=%d\n",m_vMatchUserInfo[m_nCurrentMatchID - 1].size());
			vNoSignUpUser.clear();
			for (int i=0;i<m_pGameServiceOption->wMaxPlayer;++i)
			{
				IServerUserItem * pITargerUserItem=pBindParameter->pIServerUserItem;
				if (pITargerUserItem==NULL) 
				{
					pBindParameter++;
					continue;
				}
				bool bHaveSignUp = false;
				if(m_vMatchUserInfo[m_nCurrentMatchID - 1].size()>0&&m_nCurrentMatchID>=1) 
				{
					for (int j=0;j<m_vMatchUserInfo[m_nCurrentMatchID-1].size();++j)
					{
						if (pITargerUserItem->GetUserID()==m_vMatchUserInfo[m_nCurrentMatchID-1][j].dwUserID)
						{
							bHaveSignUp = true;
						}
					}

				}
				
				if (bHaveSignUp==false)
				{
					vNoSignUpUser.push_back(pITargerUserItem);
				}
				pBindParameter++;
			}
			CMD_GRO_MatchInfoNotify stMatchInfoNotify={0};
			stMatchInfoNotify.nMatchID = pNotifyInfo->nMatchID;
			stMatchInfoNotify.nMatchPeopleNum = pNotifyInfo->nMatchPeopleNum;
			stMatchInfoNotify.nMatchScore = pNotifyInfo->nMatchScore;
			lstrcpyn(stMatchInfoNotify.szMatchPrise,pNotifyInfo->szMatchPrise,sizeof(stMatchInfoNotify.szMatchPrise));
			lstrcpyn(stMatchInfoNotify.szMatchTitle,pNotifyInfo->szNotification,sizeof(stMatchInfoNotify.szMatchTitle));
			stMatchInfoNotify.bMatchStatus = true;
			IServerUserItem * pIServerUserItem=NULL;
			if(m_vMatchUserInfo[m_nCurrentMatchID - 1].size()>0&&m_nCurrentMatchID>=1) 
			{
				for (int i=0;i<m_vMatchUserInfo[m_nCurrentMatchID-1].size();++i)
				{
					printf("报名玩家发送提示\n");
					pIServerUserItem = m_ServerUserManager.SearchUserItem(m_vMatchUserInfo[m_nCurrentMatchID-1][i].dwUserID);
					SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_MATCH_INFO_NOTIFY,&stMatchInfoNotify,sizeof(CMD_GRO_MatchInfoNotify));
				}
			}
			stMatchInfoNotify.bMatchStatus = false;
			for (int i=0;i<vNoSignUpUser.size();++i)
			{
				printf("没有报名玩家发送提示\n");
				SendData(vNoSignUpUser[i],MDM_GR_USER,SUB_GR_MATCH_INFO_NOTIFY,&stMatchInfoNotify,sizeof(CMD_GRO_MatchInfoNotify));
			}
			return true;
		}
	case SUB_CS_S_MATCH_NOTIFICATION_COUNTDOWN:
		{
			CMD_GPO_MatchTimeNotify * pNotifyInfo = (CMD_GPO_MatchTimeNotify *)pData;
			//遍历当前所有玩家，有参赛数据的为已报名
			IServerUserItem * pIServerUserItem=NULL;
			printf("收到房间5秒倒计时信息，报名玩家人数=%d, currentMatchID = %d\n",m_vMatchUserInfo[m_nCurrentMatchID-1].size(), m_nCurrentMatchID);
			for (int i=0;i<m_vMatchUserInfo[m_nCurrentMatchID-1].size();++i)
			{
				pIServerUserItem = m_ServerUserManager.SearchUserItem(m_vMatchUserInfo[m_nCurrentMatchID-1][i].dwUserID);
				printf("收到房间5秒倒计时信息，报名玩家id=%d\n",m_vMatchUserInfo[m_nCurrentMatchID-1][i].dwUserID);
				printf("比赛状态=%d,剩余秒数=%d，标题=%s\n",pNotifyInfo->cbMatchStatus,pNotifyInfo->nSecond,pNotifyInfo->szTitle);
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_MATCH_TIME_NOTIFY,pNotifyInfo,sizeof(CMD_GPO_MatchTimeNotify));
			}
			return true;
		}
	}

	return true;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_USERID:		//I D 登录
		{
			return OnTCPNetworkSubLogonUserID(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_MOBILE:		//手机登录
		{
			return OnTCPNetworkSubLogonMobile(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubLogonAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return true;
}

//用户处理
bool CAttemperEngineSink::OnTCPNetworkMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_RULE:			//用户规则
		{
			return OnTCPNetworkSubUserRule(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_LOOKON:		//用户旁观
		{
			return OnTCPNetworkSubUserLookon(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_SITDOWN:		//用户坐下
		{
			return OnTCPNetworkSubUserSitDown(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_STANDUP:		//用户起立
		{
			return OnTCPNetworkSubUserStandUp(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_CHAT:			//用户聊天
		{
			return OnTCPNetworkSubUserChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_EXPRESSION:	//用户表情
		{
			return OnTCPNetworkSubUserExpression(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_WISPER_CHAT:		//用户私聊
		{
			return OnTCPNetworkSubWisperChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_WISPER_EXPRESSION:	//私聊表情
		{
			return OnTCPNetworkSubWisperExpression(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_PROPERTY_BUY:		//购买道具
		{
			return OnTCPNetworkSubPropertyBuy(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_PROPERTY_TRUMPET:   //喇叭道具
		{
			return OnTCPNetwordSubSendTrumpet(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_INVITE_REQ:    //邀请用户
		{
			return OnTCPNetworkSubUserInviteReq(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_REPULSE_SIT:   //拒绝厌友
		{
			return OnTCPNetworkSubUserRepulseSit(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_KICK_USER:    //踢出用户
		{
			return OnTCPNetworkSubMemberKickUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_INFO_REQ:     //请求用户信息
		{
			return OnTCPNetworkSubUserInfoReq(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_CHAIR_REQ:    //请求更换位置
		{
			return OnTCPNetworkSubUserChairReq(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_CHAIR_INFO_REQ: //请求椅子用户信息
		{
			return OnTCPNetworkSubChairUserInfoReq(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_C_VIP_TRUMPET:
		{
			return OnTCPNetworkSubVIPTrumpet(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_FIRST_SUPERREWARD:		
		{
			return OnTCPNetworkSubFirstEnterSuper(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_Match_SignUP:		
		{
			return OnTCPNetworkSubMatchSignUP(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_MATCH_GET_MATCH_PRIZE:		
		{
			return OnTCPNetworkSubMatchGetPrize(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//银行处理
bool CAttemperEngineSink::OnTCPNetworkMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_INSURE_INFO:		//银行查询
		{
			return OnTCPNetworkSubQueryInsureInfo(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SAVE_SCORE_REQUEST:		//存款请求
		{
			return OnTCPNetworkSubSaveScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TAKE_SCORE_REQUEST:		//取款请求
		{
			return OnTCPNetworkSubTakeScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TRANSFER_SCORE_REQUEST:	//转账请求
		{
			return OnTCPNetworkSubTransferScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_QUERY_USER_INFO_REQUEST:	//查询用户
		{
			return OnTCPNetworkSubQueryUserInfoRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GRR_USER_TRANSER_DRAGON:
		{
			if (wDataSize!=sizeof(CMD_GP_UserTransferDragon)) return false;

			//获取用户
			WORD wBindIndex=LOWORD(dwSocketID);
			IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

			CMD_GR_UserTransferDragon * pUserTransferScore=(CMD_GR_UserTransferDragon *)pData;
			pUserTransferScore->szNickName[CountArray(pUserTransferScore->szNickName)-1]=0;
			pUserTransferScore->szPassword[CountArray(pUserTransferScore->szPassword)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			DBR_GR_UserTransferDragon UserTransferScore = {0};

			UserTransferScore.dwUserID=pUserTransferScore->dwUserID;
			UserTransferScore.cbByNickName=pUserTransferScore->cbByNickName;
			UserTransferScore.lTransferDragon=pUserTransferScore->lTransferDragon;
			UserTransferScore.dwClientAddr=pIServerUserItem->GetClientAddr();
			lstrcpyn(UserTransferScore.szNickName,pUserTransferScore->szNickName,CountArray(UserTransferScore.szNickName));
			lstrcpyn(UserTransferScore.szPassword,pUserTransferScore->szPassword,CountArray(UserTransferScore.szPassword));
			lstrcpyn(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,CountArray(UserTransferScore.szMachineID));

			m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TRANSFER_DRAGON,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GRR_USER_USE_DRAGON:
		{
			//效验参数
			if (wDataSize!=sizeof(CMD_GR_UserUseDragon)) return false;

			//获取用户
			WORD wBindIndex=LOWORD(dwSocketID);
			IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

			CMD_GR_UserUseDragon * pUserTakeScore=(CMD_GR_UserUseDragon *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			DBR_GR_UserUseDragon UserTakeScore = {0};

			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lUseDragon=pUserTakeScore->lUseDragon;
			UserTakeScore.dwClientAddr=pIServerUserItem->GetClientAddr();
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//投递请求
			m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_USE_DRAGON,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	}

	return false;
}

//管理处理
bool CAttemperEngineSink::OnTCPNetworkMainManage(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_OPTION:		//查询设置
		{
			return OnTCPNetworkSubQueryOption(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_OPTION_SERVER:		//房间设置
		{
			return OnTCPNetworkSubOptionServer(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KILL_USER:          //踢出用户
		{
			return OnTCPNetworkSubManagerKickUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LIMIT_USER_CHAT:	//限制聊天
		{
			return OnTCPNetworkSubLimitUserChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KICK_ALL_USER:		//踢出用户
		{
			return OnTCPNetworkSubKickAllUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SEND_MESSAGE:		//发布消息
		{
			return OnTCPNetworkSubSendMessage(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_DISMISSGAME:        //解散游戏
		{
			return OnTCPNetworkSubDismissGame(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//比赛命令
bool CAttemperEngineSink::OnTCPNetworkMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//效验接口
	ASSERT(m_pIGameMatchServiceManager!=NULL);
	if (m_pIGameMatchServiceManager==NULL) return false;

	//消息处理
	return m_pIGameMatchServiceManager->OnEventSocketMatch(wSubCmdID,pData,wDataSize,pIServerUserItem,dwSocketID);
}

//游戏处理
bool CAttemperEngineSink::OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//处理过虑
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//消息处理 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	return pTableFrame->OnEventSocketGame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//框架处理
bool CAttemperEngineSink::OnTCPNetworkMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//处理过虑
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//消息处理 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	return pTableFrame->OnEventSocketFrame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//I D 登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonUserID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonUserID));
	if (wDataSize<sizeof(CMD_GR_LogonUserID)) return false;

	//处理消息
	CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
	pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
	pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

	//绑定信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//重复判断
	if ((pBindParameter==NULL)||(pIBindUserItem!=NULL))
	{ 
		ASSERT(FALSE);
		return false;
	}

	//房间判断
	if(pLogonUserID->wKindID != m_pGameServiceOption->wKindID)
	{
		//发送失败
		SendLogonFailure(TEXT("很抱歉，此游戏房间已经关闭了，不允许继续进入！"),LOGON_FAIL_SERVER_INVALIDATION,dwSocketID);
		return true;
	}

	//机器人和真人不许互踢
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonUserID->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
			|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
		{
			SendRoomMessage(dwSocketID, TEXT("该账号已在此房间游戏，且不允许踢出，请咨询管理员！"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
			return true;
		}
	}

	//版本信息
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonUserID->dwPlazaVersion;
	pBindParameter->dwFrameVersion=pLogonUserID->dwFrameVersion;
	pBindParameter->dwProcessVersion=pLogonUserID->dwProcessVersion;

	//大厅版本
	DWORD dwPlazaVersion=pLogonUserID->dwPlazaVersion;
	DWORD dwFrameVersion=pLogonUserID->dwFrameVersion;
	DWORD dwClientVersion=pLogonUserID->dwProcessVersion;
	if (PerformCheckVersion(dwPlazaVersion,dwFrameVersion,dwClientVersion,dwSocketID)==false) return true;

	//切换判断
	if((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonUserID->szPassword)==true))
	{
		SwitchUserItemConnect(pIServerUserItem,pLogonUserID->szMachineID,wBindIndex);
		return true;
	}

	if(m_pIGameMatchServiceManager!=NULL&&m_pIGameMatchServiceManager->OnEventEnterMatch(dwSocketID,(VOID*)pLogonUserID, pBindParameter->dwClientAddr))
	{
		//SendRoomMessage(dwSocketID, szPrint, SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
		return true;
	}
	//变量定义
	DBR_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//构造数据
	LogonUserID.dwUserID=pLogonUserID->dwUserID;
	LogonUserID.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
	lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonUserID.dwUserID,DBR_GR_LOGON_USERID,dwSocketID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//手机登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonMobile));
	if (wDataSize<sizeof(CMD_GR_LogonMobile)) return false;

	//处理消息
	CMD_GR_LogonMobile * pLogonMobile=(CMD_GR_LogonMobile *)pData;
	pLogonMobile->szPassword[CountArray(pLogonMobile->szPassword)-1]=0;
	pLogonMobile->szMachineID[CountArray(pLogonMobile->szMachineID)-1]=0;

	//绑定信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//重复判断
	if ((pBindParameter==NULL)||(pIBindUserItem!=NULL))
	{ 
		ASSERT(FALSE);
		return false;
	}

	//机器人和真人不许互踢
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonMobile->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
			|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
		{
			SendRoomMessage(dwSocketID, TEXT("该账号已在此房间游戏，且不允许踢出，请咨询管理员！"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
			return false;
		}
	}

	//版本信息
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->dwProcessVersion=pLogonMobile->dwProcessVersion;

	//大厅版本
	DWORD dwClientVersion=pLogonMobile->dwProcessVersion;
	if (PerformCheckVersion(0L,0L,dwClientVersion,dwSocketID)==false) return true;

	//切换判断
	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonMobile->szPassword)==true))
	{
		SwitchUserItemConnect(pIServerUserItem,pLogonMobile->szMachineID,wBindIndex,pLogonMobile->cbDeviceType,pLogonMobile->wBehaviorFlags,pLogonMobile->wPageTableCount);
		return true;
	}

	//变量定义
	DBR_GR_LogonMobile LogonMobile;
	ZeroMemory(&LogonMobile,sizeof(LogonMobile));

	//构造数据
	LogonMobile.dwUserID=pLogonMobile->dwUserID;
	LogonMobile.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonMobile.szPassword,pLogonMobile->szPassword,CountArray(LogonMobile.szPassword));
	lstrcpyn(LogonMobile.szMachineID,pLogonMobile->szMachineID,CountArray(LogonMobile.szMachineID));
	LogonMobile.cbDeviceType=pLogonMobile->cbDeviceType;
	LogonMobile.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
	LogonMobile.wPageTableCount=pLogonMobile->wPageTableCount;

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonMobile.dwUserID,DBR_GR_LOGON_MOBILE,dwSocketID,&LogonMobile,sizeof(LogonMobile));

	return true;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonAccounts));
	if (wDataSize<=sizeof(CMD_GR_LogonAccounts)) return false;

	//处理消息
	CMD_GR_LogonAccounts * pLogonAccounts=(CMD_GR_LogonAccounts *)pData;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//绑定信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//重复判断
	if (pIBindUserItem!=NULL)
	{ 
		ASSERT(FALSE);
		return false;
	}

	//机器人和真人不许互踢
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonAccounts->szAccounts);
	if (pIServerUserItem!=NULL)
	{
		if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
			|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
		{
			SendRoomMessage(dwSocketID, TEXT("该账号已在此房间游戏，且不允许踢出，请咨询管理员！"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
			return false;
		}
	}

	//版本信息
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;
	pBindParameter->dwFrameVersion=pLogonAccounts->dwFrameVersion;
	pBindParameter->dwProcessVersion=pLogonAccounts->dwProcessVersion;

	//大厅版本
	DWORD dwPlazaVersion=pLogonAccounts->dwPlazaVersion;
	DWORD dwFrameVersion=pLogonAccounts->dwFrameVersion;
	DWORD dwClientVersion=pLogonAccounts->dwProcessVersion;
	if (PerformCheckVersion(dwPlazaVersion,dwFrameVersion,dwClientVersion,dwSocketID)==false) return true;

	//切换判断
	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonAccounts->szPassword)==true))
	{
		SwitchUserItemConnect(pIServerUserItem,pLogonAccounts->szMachineID,wBindIndex);
		return true;
	}

	//变量定义
	DBR_GR_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//构造数据
	LogonAccounts.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//用户规则
bool CAttemperEngineSink::OnTCPNetworkSubUserRule(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GR_UserRule));
	if (wDataSize<sizeof(CMD_GR_UserRule)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//规则判断
	//ASSERT(CServerRule::IsForfendGameRule(m_pGameServiceOption->dwServerRule)==false);
	if (CServerRule::IsForfendGameRule(m_pGameServiceOption->dwServerRule)==true) return true;

	//消息处理
	tagUserRule * pUserRule=pIServerUserItem->GetUserRule();
	CMD_GR_UserRule * pCMDUserRule=(CMD_GR_UserRule *)pData;

	//规则标志
	pUserRule->bLimitSameIP=((pCMDUserRule->cbRuleMask&UR_LIMIT_SAME_IP)>0);
	pUserRule->bLimitWinRate=((pCMDUserRule->cbRuleMask&UR_LIMIT_WIN_RATE)>0);
	pUserRule->bLimitFleeRate=((pCMDUserRule->cbRuleMask&UR_LIMIT_FLEE_RATE)>0);
	pUserRule->bLimitGameScore=((pCMDUserRule->cbRuleMask&UR_LIMIT_GAME_SCORE)>0);

	//规则属性
	pUserRule->szPassword[0]=0;
	pUserRule->wMinWinRate=pCMDUserRule->wMinWinRate;
	pUserRule->wMaxFleeRate=pCMDUserRule->wMaxFleeRate;
	pUserRule->lMaxGameScore=pCMDUserRule->lMaxGameScore;
	pUserRule->lMinGameScore=pCMDUserRule->lMinGameScore;

	//桌子密码
	if (wDataSize>sizeof(CMD_GR_UserRule))
	{
		//变量定义
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pCMDUserRule+1,wDataSize-sizeof(CMD_GR_UserRule));

		//提取处理
		while (true)
		{
			//提取数据
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;

			//数据分析
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_TABLE_PASSWORD:		//桌子密码
				{
					//效验数据
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(pUserRule->szPassword));

					//规则判断
					ASSERT(CServerRule::IsForfendLockTable(m_pGameServiceOption->dwServerRule)==false);
					if (CServerRule::IsForfendLockTable(m_pGameServiceOption->dwServerRule)==true) break;

					//设置数据
					if (DataDescribe.wDataSize<=sizeof(pUserRule->szPassword))
					{
						CopyMemory(&pUserRule->szPassword,pDataBuffer,DataDescribe.wDataSize);
						pUserRule->szPassword[CountArray(pUserRule->szPassword)-1]=0;
					}

					break;
				}
			}
		}
	}

	return true;
}

//用户旁观
bool CAttemperEngineSink::OnTCPNetworkSubUserLookon(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserLookon));
	if (wDataSize!=sizeof(CMD_GR_UserLookon)) return false;

	//效验数据
	CMD_GR_UserLookon * pUserLookon=(CMD_GR_UserLookon *)pData;
	if (pUserLookon->wChairID>=m_pGameServiceAttrib->wChairCount) return false;
	if (pUserLookon->wTableID>=(WORD)m_TableFrameArray.GetCount()) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//消息处理
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	if ((wTableID==pUserLookon->wTableID)&&(wChairID==pUserLookon->wChairID)&&(cbUserStatus==US_LOOKON)) return true;

	//用户判断
	if (cbUserStatus==US_PLAYING)
	{
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),0);
		return true;
	}

	//离开处理
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	//坐下处理
	CTableFrame * pTableFrame=m_TableFrameArray[pUserLookon->wTableID];
	pTableFrame->PerformLookonAction(pUserLookon->wChairID,pIServerUserItem);

	return true;
}

//用户坐下
bool CAttemperEngineSink::OnTCPNetworkSubUserSitDown(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserSitDown));
	if (wDataSize!=sizeof(CMD_GR_UserSitDown)) return false;

	//效验数据
	CMD_GR_UserSitDown * pUserSitDown=(CMD_GR_UserSitDown *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//消息处理
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//重复判断
	if ((pUserSitDown->wTableID<m_pGameServiceOption->wTableCount)&&(pUserSitDown->wChairID<m_pGameServiceAttrib->wChairCount))
	{
		CTableFrame * pTableFrame=m_TableFrameArray[pUserSitDown->wTableID];
		if (pTableFrame->GetTableUserItem(pUserSitDown->wChairID)==pIServerUserItem) return true;
	}

	//用户判断
	if (cbUserStatus==US_PLAYING)
	{
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),0);
		return true;
	}

	//插入分组
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
	{
		//返回FALSE为 定时开赛
		if(InsertDistribute(pIServerUserItem))
			return true;
	}

	//离开处理
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	//防作弊
	if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
	{
		if(m_TableFrameArray[0]->EfficacyEnterTableScoreRule(0, pIServerUserItem))
			InsertWaitDistribute(pIServerUserItem);

		return true;
	}

	//请求调整
	WORD wRequestTableID=pUserSitDown->wTableID;
	WORD wRequestChairID=pUserSitDown->wChairID;

	//桌子调整
	if (wRequestTableID>=m_TableFrameArray.GetCount())
	{
		//起始桌子
		WORD wStartTableID=0;
		DWORD dwServerRule=m_pGameServiceOption->dwServerRule;
		if ((CServerRule::IsAllowAvertCheatMode(dwServerRule)==true)&&(m_pGameServiceAttrib->wChairCount<MAX_CHAIR)) wStartTableID=1;

		//动态加入
		bool bDynamicJoin=true;
		if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
		if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

		//寻找位置
		for (WORD i=wStartTableID;i<m_TableFrameArray.GetCount();i++)
		{
			//游戏状态
			if ((m_TableFrameArray[i]->IsGameStarted()==true)&&(bDynamicJoin==false))continue;

			//获取空位
			WORD wNullChairID=m_TableFrameArray[i]->GetNullChairID();

			//调整结果
			if (wNullChairID!=INVALID_CHAIR)
			{
				//设置变量
				wRequestTableID=i;
				wRequestChairID=wNullChairID;

				break;
			}
		}

		//结果判断
		if ((wRequestTableID==INVALID_CHAIR)||(wRequestChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("当前游戏房间已经人满为患了，暂时没有可以让您加入的位置，请稍后再试！"),0);
			return true;
		}
	}

	//椅子调整
	if (wRequestChairID>=m_pGameServiceAttrib->wChairCount)
	{
		//效验参数
		ASSERT(wRequestTableID<m_TableFrameArray.GetCount());
		if (wRequestTableID>=m_TableFrameArray.GetCount()) return false;

		//查找空位
		wRequestChairID=m_TableFrameArray[wRequestTableID]->GetNullChairID();

		//结果判断
		if (wRequestChairID==INVALID_CHAIR)
		{
			SendRequestFailure(pIServerUserItem,TEXT("由于此游戏桌暂时没有可以让您加入的位置了，请选择另外的游戏桌！"),0);
			return true;
		}
	}

	//坐下处理
	CTableFrame * pTableFrame=m_TableFrameArray[wRequestTableID];
	pTableFrame->PerformSitDownAction(wRequestChairID,pIServerUserItem,pUserSitDown->szPassword);

	return true;
}

//用户起立
bool CAttemperEngineSink::OnTCPNetworkSubUserStandUp(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserStandUp));
	if (wDataSize!=sizeof(CMD_GR_UserStandUp)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//定义变量
	CMD_GR_UserStandUp * pUserStandUp=(CMD_GR_UserStandUp *)pData;

	//取消分组
	if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
	{
		POSITION pos=m_WaitDistributeList.GetHeadPosition();
		while(pos != NULL)
		{
			POSITION tempPos=pos;
			IServerUserItem *pUserItem = m_WaitDistributeList.GetNext(pos);
			if(pUserItem->GetUserID()==pIServerUserItem->GetUserID())
			{
				m_WaitDistributeList.RemoveAt(tempPos);
				break;
			}
		}

		if(pUserStandUp->wTableID==INVALID_TABLE) return true;
	}

	//效验数据
	if (pUserStandUp->wChairID>=m_pGameServiceAttrib->wChairCount) return false;
	if (pUserStandUp->wTableID>=(WORD)m_TableFrameArray.GetCount()) return false;

	//消息处理
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID!=pUserStandUp->wTableID)||(wChairID!=pUserStandUp->wChairID)) return true;

	//用户判断
	if ((pUserStandUp->cbForceLeave==FALSE)&&(pIServerUserItem->GetUserStatus()==US_PLAYING))
	{
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),0);
		return true;
	}

	//离开处理
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GR_USER, 500, NULL, 0);

	return true;
}

//用户聊天
bool CAttemperEngineSink::OnTCPNetworkSubUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//变量定义
	CMD_GR_C_UserChat * pUserChat=(CMD_GR_C_UserChat *)pData;

	//效验参数
	ASSERT(wDataSize<=sizeof(CMD_GR_C_UserChat));
	ASSERT(wDataSize>=(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//效验参数
	if (wDataSize>sizeof(CMD_GR_C_UserChat)) return false;
	if (wDataSize<(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	IServerUserItem * pIRecvUserItem=NULL;
	if (pUserChat->dwTargetUserID!=0)
	{
		pIRecvUserItem=m_ServerUserManager.SearchUserItem(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;
	}

	//状态判断
	if ((CServerRule::IsForfendRoomChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户大厅聊天！"),SMT_CHAT);
		return true;
	}

	//权限判断
	if (CUserRight::CanRoomChat(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有大厅发言的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//构造消息
	CMD_GR_S_UserChat UserChat;
	ZeroMemory(&UserChat,sizeof(UserChat));

	//字符过滤
	SensitiveWordFilter(pUserChat->szChatString,UserChat.szChatString,CountArray(UserChat.szChatString));
	
	//构造数据
	UserChat.dwChatColor=pUserChat->dwChatColor;
	UserChat.wChatLength=pUserChat->wChatLength;
	UserChat.dwSendUserID=pISendUserItem->GetUserID();
	UserChat.dwTargetUserID=pUserChat->dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//转发消息
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//用户表情
bool CAttemperEngineSink::OnTCPNetworkSubUserExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_C_UserExpression)) return false;

	//变量定义
	CMD_GR_C_UserExpression * pUserExpression=(CMD_GR_C_UserExpression *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	IServerUserItem * pIRecvUserItem=NULL;
	if (pUserExpression->dwTargetUserID!=0)
	{
		pIRecvUserItem=m_ServerUserManager.SearchUserItem(pUserExpression->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;
	}

	//状态判断
	if ((CServerRule::IsForfendRoomChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户大厅聊天！"),SMT_CHAT);
		return true;
	}

	//权限判断
	if (CUserRight::CanRoomChat(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有大厅发言的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//构造消息
	CMD_GR_S_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//构造数据
	UserExpression.wItemIndex=pUserExpression->wItemIndex;
	UserExpression.dwSendUserID=pISendUserItem->GetUserID();
	UserExpression.dwTargetUserID=pUserExpression->dwTargetUserID;

	//转发消息
	SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//用户私聊
bool CAttemperEngineSink::OnTCPNetworkSubWisperChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//变量定义
	CMD_GR_C_WisperChat * pWisperChat=(CMD_GR_C_WisperChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	IServerUserItem * pIRecvUserItem=m_ServerUserManager.SearchUserItem(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//状态判断
	if ((CServerRule::IsForfendWisperChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户私聊！"),SMT_CHAT);
		return true;
	}

	//同桌判断
	if ((CServerRule::IsForfendWisperOnGame(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		//变量定义
		bool bForfend=true;
		WORD wTableIDSend=pISendUserItem->GetTableID();
		WORD wTableIDRecv=pIRecvUserItem->GetTableID();

		//规则判断
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pISendUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&((wTableIDSend==INVALID_TABLE)||(wTableIDSend!=wTableIDRecv))) bForfend=false;

		//提示消息
		if (bForfend==true)
		{
			SendRoomMessage(pISendUserItem,TEXT("抱歉，此游戏房间不允许在游戏中与同桌的玩家私聊！"),SMT_EJECT|SMT_CHAT);
			return true;
		}
	}

	//权限判断
	if (CUserRight::CanWisper(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有发送私聊的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//变量定义
	CMD_GR_S_WisperChat WisperChat;
	ZeroMemory(&WisperChat,sizeof(WisperChat));

	//字符过滤
	SensitiveWordFilter(pWisperChat->szChatString,WisperChat.szChatString,CountArray(WisperChat.szChatString));

	//构造数据
	WisperChat.dwChatColor=pWisperChat->dwChatColor;
	WisperChat.wChatLength=pWisperChat->wChatLength;
	WisperChat.dwSendUserID=pISendUserItem->GetUserID();
	WisperChat.dwTargetUserID=pIRecvUserItem->GetUserID();
	WisperChat.wChatLength=CountStringBuffer(WisperChat.szChatString);

	//转发消息
	WORD wHeadSize=sizeof(WisperChat)-sizeof(WisperChat.szChatString);
	SendData(pISendUserItem,MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));
	SendData(pIRecvUserItem,MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));

	return true;
}

//用户表情
bool CAttemperEngineSink::OnTCPNetworkSubWisperExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_C_WisperExpression)) return false;

	//变量定义
	CMD_GR_C_WisperExpression * pWisperExpression=(CMD_GR_C_WisperExpression *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	IServerUserItem * pIRecvUserItem=m_ServerUserManager.SearchUserItem(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//状态判断
	if ((CServerRule::IsForfendWisperChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户私聊！"),SMT_CHAT);
		return true;
	}

	//同桌判断
	if ((CServerRule::IsForfendWisperOnGame(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		//变量定义
		bool bForfend=true;
		WORD wTableIDSend=pISendUserItem->GetTableID();
		WORD wTableIDRecv=pIRecvUserItem->GetTableID();

		//规则判断
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pISendUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&((wTableIDSend==INVALID_TABLE)||(wTableIDSend!=wTableIDRecv))) bForfend=false;

		//提示消息
		if (bForfend==true)
		{
			SendRoomMessage(pISendUserItem,TEXT("抱歉，此游戏房间不允许在游戏中与同桌的玩家私聊！"),SMT_EJECT|SMT_CHAT);
			return true;
		}
	}

	//权限判断
	if (CUserRight::CanWisper(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有发送私聊的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//变量定义
	CMD_GR_S_WisperExpression WisperExpression;
	ZeroMemory(&WisperExpression,sizeof(WisperExpression));

	//构造数据
	WisperExpression.wItemIndex=pWisperExpression->wItemIndex;
	WisperExpression.dwSendUserID=pISendUserItem->GetUserID();
	WisperExpression.dwTargetUserID=pWisperExpression->dwTargetUserID;

	//转发消息
	SendData(pISendUserItem,MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));
	SendData(pIRecvUserItem,MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));

	return true;
}

//购买道具
bool CAttemperEngineSink::OnTCPNetworkSubPropertyBuy(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//事件处理
	int cbResult = OnPropertyBuy(pData, wDataSize, dwSocketID);

	//结果判断
	if(cbResult == RESULT_ERROR) return false;
	if(cbResult == RESULT_FAIL) return true;
	if(cbResult == RESULT_SUCCESS) return true;

	return true;
}

//购买道具
int CAttemperEngineSink::OnPropertyBuy(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_PropertyBuy));
	if (wDataSize!=sizeof(CMD_GR_C_PropertyBuy)) return RESULT_ERROR;

	//变量定义
	CMD_GR_C_PropertyBuy * pPropertyBuy=(CMD_GR_C_PropertyBuy *)pData;

	//数据效验
	ASSERT(pPropertyBuy->wItemCount>0);
	if (pPropertyBuy->wItemCount==0) return RESULT_ERROR;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return RESULT_ERROR;

	//目标用户
	IServerUserItem * pITargerUserItem=m_ServerUserManager.SearchUserItem(pPropertyBuy->dwTargetUserID);
	if (pITargerUserItem==NULL) return RESULT_ERROR;

	//比赛房间
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		//发送消息
		SendPropertyFailure(pIServerUserItem,TEXT("比赛房间不可以使用此功能！"),0L,pPropertyBuy->cbRequestArea);

		return RESULT_FAIL;
	}

	//练习房间
	if (m_pGameServiceOption->wServerType==GAME_GENRE_EDUCATE)
	{
		SendPropertyFailure(pIServerUserItem,TEXT("练习房间不可以使用此功能！"),0L,pPropertyBuy->cbRequestArea);
		return RESULT_FAIL;
	}

	//购前事件
	if(OnEventPropertyBuyPrep(pPropertyBuy->cbRequestArea,pPropertyBuy->wPropertyIndex,pIServerUserItem,pITargerUserItem)==false)
		return RESULT_FAIL;

	//变量定义
	DBR_GR_PropertyRequest PropertyRequest;
	ZeroMemory(&PropertyRequest,sizeof(PropertyRequest));

	//查找道具
	tagPropertyInfo * pPropertyInfo=m_GamePropertyManager.SearchPropertyItem(pPropertyBuy->wPropertyIndex);
	if (pPropertyInfo==NULL)
	{
		SendPropertyFailure(pIServerUserItem,TEXT("您购买的道具不存在或在维护中，请与管理员联系！"),0L,pPropertyBuy->cbRequestArea);
		return RESULT_FAIL;
	}

	//消费方式
	if (pPropertyBuy->cbConsumeScore==TRUE)
	{
		//房间判断
		ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
		if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return RESULT_FAIL;		

		//锁定计算
		PropertyRequest.cbConsumeScore=TRUE;
		PropertyRequest.lFrozenedScore=pPropertyInfo->lPropertyGold*pPropertyBuy->wItemCount;

		//会员折扣
		if (pIServerUserItem->GetMemberOrder()>0)
		{
			PropertyRequest.lFrozenedScore = PropertyRequest.lFrozenedScore*pPropertyInfo->wDiscount/100L;
		}

		//锁定积分
		if (pIServerUserItem->FrozenedUserScore(PropertyRequest.lFrozenedScore)==false)
		{
			SendPropertyFailure(pIServerUserItem,TEXT("您的游戏币余额不足，道具购买失败！"),0L,pPropertyBuy->cbRequestArea);
			return RESULT_FAIL;
		}
	}
	else
	{
		//银行扣费
		PropertyRequest.lFrozenedScore=0L;
		PropertyRequest.cbConsumeScore=FALSE;

		//变量定义
		SCORE lInsure = pIServerUserItem->GetUserInsure();
		SCORE lConsumeScore = pPropertyInfo->lPropertyGold*pPropertyBuy->wItemCount;

		//会员折扣
		if (pIServerUserItem->GetMemberOrder()>0)
		{
			lConsumeScore = lConsumeScore*pPropertyInfo->wDiscount/100L;
		}

		//银行校验
		if(lInsure < lConsumeScore)
		{
			SendPropertyFailure(pIServerUserItem,TEXT("您的保险柜余额不足，请存款后再次购买！"),0L,pPropertyBuy->cbRequestArea);
			return RESULT_FAIL;
		}		
	}

	//购买信息
	PropertyRequest.cbRequestArea=pPropertyBuy->cbRequestArea;
	PropertyRequest.wItemCount=pPropertyBuy->wItemCount;
	PropertyRequest.wPropertyIndex=pPropertyBuy->wPropertyIndex;
	PropertyRequest.dwSourceUserID=pIServerUserItem->GetUserID();
	PropertyRequest.dwTargetUserID=pITargerUserItem->GetUserID();

	//设置权限
	if(PropertyRequest.wPropertyIndex==PROPERTY_ID_VIPROOM_CARD)
		PropertyRequest.dwUserRight |= UR_GAME_KICK_OUT_USER|UR_GAME_ENTER_VIP_ROOM;
	else if(PropertyRequest.wPropertyIndex>=PROPERTY_ID_BLUERING_CARD && PropertyRequest.wPropertyIndex <=PROPERTY_ID_REDRING_CARD)
		PropertyRequest.dwUserRight |= UR_GAME_KICK_OUT_USER;
	else
		PropertyRequest.dwUserRight=0;

	//系统信息
	PropertyRequest.wTableID=INVALID_TABLE;
	PropertyRequest.dwInoutIndex=pIServerUserItem->GetInoutIndex();
	PropertyRequest.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(PropertyRequest.szMachineID,pIServerUserItem->GetMachineID(),CountArray(PropertyRequest.szMachineID));

	//投递数据
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_PROPERTY_REQUEST,dwSocketID,&PropertyRequest,sizeof(PropertyRequest));

	return RESULT_SUCCESS;
}

//使用道具
bool CAttemperEngineSink::OnTCPNetwordSubSendTrumpet(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_SendTrumpet));
	if (wDataSize!=sizeof(CMD_GR_C_SendTrumpet)) return false;

	//变量定义
	CMD_GR_C_SendTrumpet * pSendTrumpet=(CMD_GR_C_SendTrumpet *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if(pIServerUserItem==NULL) return false;

	//构造结构
	CMD_GR_C_PropertyBuy  PropertyBuy;
	PropertyBuy.cbRequestArea=pSendTrumpet->cbRequestArea;
	PropertyBuy.dwTargetUserID=pIServerUserItem->GetUserID();
	PropertyBuy.wPropertyIndex=pSendTrumpet->wPropertyIndex;
	PropertyBuy.cbConsumeScore=FALSE;
	PropertyBuy.wItemCount=1;
    
	//购买
	int cbResult = OnPropertyBuy((void *)&PropertyBuy,sizeof(PropertyBuy),dwSocketID);

	//结果判断
	if(cbResult == RESULT_ERROR) return false;
	if(cbResult != RESULT_SUCCESS) return true;

	//获取道具
	tagUserProperty * pUserProperty = pIServerUserItem->GetUserProperty();

	//道具索引
	BYTE cbIndex=(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?2:3;

	//构造结构
	CMD_GR_S_SendTrumpet  SendTrumpet;
	SendTrumpet.dwSendUserID=pIServerUserItem->GetUserID();
	SendTrumpet.wPropertyIndex=pSendTrumpet->wPropertyIndex;
	SendTrumpet.TrumpetColor=pSendTrumpet->TrumpetColor;
	ZeroMemory(SendTrumpet.szTrumpetContent,sizeof(SendTrumpet.szTrumpetContent));
	CopyMemory(SendTrumpet.szSendNickName,pIServerUserItem->GetNickName(),sizeof(SendTrumpet.szSendNickName));

	//字符过滤
	SensitiveWordFilter(pSendTrumpet->szTrumpetContent,SendTrumpet.szTrumpetContent,CountArray(SendTrumpet.szTrumpetContent));

    //房间转发
	if(cbIndex==3)
	{
		//广播房间
		WORD wUserIndex=0;
		CMD_CS_S_SendTrumpet * pSendTrumpet = (CMD_CS_S_SendTrumpet *)&SendTrumpet;
		if(m_pITCPSocketService)
		{
			m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_PROPERTY_TRUMPET,pSendTrumpet,sizeof(CMD_CS_S_SendTrumpet));
		}
	}

	//游戏转发
	if(cbIndex==2)
	{
		//发送数据
		m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_PROPERTY_TRUMPET,&SendTrumpet,sizeof(SendTrumpet),BG_COMPUTER);
	}

	return true;
}

//使用道具
bool CAttemperEngineSink::OnTCPNetworkSubVIPTrumpet(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
printf("%s %d\n", __FUNCTION__, __LINE__);
	//效验参数
	if (wDataSize!=sizeof(CMD_GR_C_SendTrumpet))
	{
		printf("%s %d\n", __FUNCTION__, __LINE__);
		return false;
	}

	//变量定义
	CMD_GR_C_SendTrumpet * pSendTrumpet=(CMD_GR_C_SendTrumpet *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if(pIServerUserItem==NULL)
	{
		printf("%s %d\n", __FUNCTION__, __LINE__);
		return false;
	}

	DBR_GR_CheckTrumpet CheckTrumpet = {0};

	CheckTrumpet.dwUserID = pIServerUserItem->GetUserID();
	CheckTrumpet.cbRequestArea = pSendTrumpet->cbRequestArea;
	CopyMemory(CheckTrumpet.szTrumpetContent, pSendTrumpet->szTrumpetContent, sizeof(CheckTrumpet.szTrumpetContent));
	CheckTrumpet.TrumpetColor = pSendTrumpet->TrumpetColor;
	CheckTrumpet.wPropertyIndex = pSendTrumpet->wPropertyIndex;

	printf("%s %d\n", __FUNCTION__, __LINE__);
	//投递数据
	m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_CHECK_TRUMPET,dwSocketID,&CheckTrumpet,sizeof(CheckTrumpet));

	return true;
}

//邀请用户
bool CAttemperEngineSink::OnTCPNetworkSubUserInviteReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_GR_UserInviteReq));
	if (wDataSize!=sizeof(CMD_GR_UserInviteReq)) return false;

	//消息处理
	CMD_GR_UserInviteReq * pUserInviteReq=(CMD_GR_UserInviteReq *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//效验状态
	if (pUserInviteReq->wTableID==INVALID_TABLE) return true;
	if (pIServerUserItem->GetTableID()!=pUserInviteReq->wTableID) return true;
	if (pIServerUserItem->GetUserStatus()==US_PLAYING) return true;
	if (pIServerUserItem->GetUserStatus()==US_OFFLINE) return true;

	//目标用户
	IServerUserItem * pITargetUserItem=m_ServerUserManager.SearchUserItem(pUserInviteReq->dwUserID);
	if (pITargetUserItem==NULL) return true;
	if (pITargetUserItem->GetUserStatus()==US_PLAYING) return true;

	//发送消息
	CMD_GR_UserInvite UserInvite;
	memset(&UserInvite,0,sizeof(UserInvite));
	UserInvite.wTableID=pUserInviteReq->wTableID;
	UserInvite.dwUserID=pIServerUserItem->GetUserID();
	SendData(pITargetUserItem,MDM_GR_USER,SUB_GR_USER_INVITE,&UserInvite,sizeof(UserInvite));

	return true;
}

//拒绝厌友
bool CAttemperEngineSink::OnTCPNetworkSubUserRepulseSit(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_GR_UserRepulseSit));
	if (wDataSize!=sizeof(CMD_GR_UserRepulseSit)) return false;

	//消息处理
	CMD_GR_UserRepulseSit * pUserRepulseSit=(CMD_GR_UserRepulseSit *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//获取桌子
	CTableFrame * pTableFrame=m_TableFrameArray[pUserRepulseSit->wTableID];
	if (pTableFrame->IsGameStarted()==true) return true;

	//获取用户
	IServerUserItem * pRepulseIServerUserItem = pTableFrame->GetTableUserItem(pUserRepulseSit->wChairID);
	if (pRepulseIServerUserItem==NULL) return true;
	if(pRepulseIServerUserItem->GetUserID() != pUserRepulseSit->dwRepulseUserID)return true;

	//发送消息
	TCHAR szDescribe[256]=TEXT("");
	lstrcpyn(szDescribe,TEXT("此桌有玩家设置了不与您同桌游戏！"),CountArray(szDescribe));
	SendRoomMessage(pRepulseIServerUserItem,szDescribe,SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME);

	//弹起玩家
	pTableFrame->PerformStandUpAction(pRepulseIServerUserItem);

	return true;
}

//踢出命令
bool CAttemperEngineSink::OnTCPNetworkSubMemberKickUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_KickUser));
	if (wDataSize!=sizeof(CMD_GR_KickUser)) return false;

	//变量定义
	CMD_GR_KickUser * pKickUser=(CMD_GR_KickUser *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//目标用户
	IServerUserItem * pITargetUserItem = m_ServerUserManager.SearchUserItem(pKickUser->dwTargetUserID);
	if(pITargetUserItem==NULL) return true;

	//用户效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMemberOrder()>pITargetUserItem->GetMemberOrder()));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMemberOrder()<=pITargetUserItem->GetMemberOrder())) return false;

	//权限判断
	ASSERT(CUserRight::CanKillOutUser(pIServerUserItem->GetUserRight())==true);
	if (CUserRight::CanKillOutUser(pIServerUserItem->GetUserRight())==false) return false;

	//百人游戏
	if(m_pGameServiceAttrib->wChairCount >= MAX_CHAIR)
	{
		//发送消息
		SendRoomMessage(pIServerUserItem,TEXT("很抱歉，百人游戏不许踢人！"),SMT_EJECT);
		return true;
	}

	//用户状态
	if(pITargetUserItem->GetUserStatus()==US_PLAYING)
	{
		//变量定义
		TCHAR szMessage[256]=TEXT("");
		_sntprintf(szMessage,CountArray(szMessage),TEXT("由于玩家 [ %s ] 正在游戏中,您不能将它踢出游戏！"),pITargetUserItem->GetNickName());

		//发送消息
		SendRoomMessage(pIServerUserItem,szMessage,SMT_EJECT);
		return true;
	}

	//防踢判断
	if((pITargetUserItem->GetUserProperty()->wPropertyUseMark&PT_USE_MARK_GUARDKICK_CARD)!=0)
	{
		//变量定义
		DWORD dwCurrentTime=(DWORD)time(NULL);
		tagUserProperty * pUserProperty = pITargetUserItem->GetUserProperty();

		//时效判断
		DWORD dwValidTime=pUserProperty->PropertyInfo[2].wPropertyCount*pUserProperty->PropertyInfo[2].dwValidNum;
		if(pUserProperty->PropertyInfo[2].dwEffectTime+dwValidTime>dwCurrentTime)
		{
			//变量定义
			TCHAR szMessage[256]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("由于玩家 [ %s ] 正在使用防踢卡,您无法将它踢出游戏！"),pITargetUserItem->GetNickName());

			//发送消息
			SendRoomMessage(pIServerUserItem,szMessage,SMT_EJECT);

			return true; 
		}
		else
			pUserProperty->wPropertyUseMark &= ~PT_USE_MARK_GUARDKICK_CARD;
	}

	//请离桌子
	WORD wTargerTableID = pITargetUserItem->GetTableID();
	if(wTargerTableID != INVALID_TABLE)
	{
		//定义变量
		TCHAR szMessage[64]=TEXT("");
		_sntprintf(szMessage,CountArray(szMessage),TEXT("你已被%s请离桌子！"),pIServerUserItem->GetNickName());

		//发送消息
		SendGameMessage(pITargetUserItem,szMessage,SMT_CHAT|SMT_CLOSE_GAME);

		CTableFrame * pTableFrame=m_TableFrameArray[wTargerTableID];
		if (pTableFrame->PerformStandUpAction(pITargetUserItem)==false) return true;
	}

	return true;
}

//请求用户信息
bool CAttemperEngineSink::OnTCPNetworkSubUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserInfoReq));
	if (wDataSize!=sizeof(CMD_GR_UserInfoReq)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_UserInfoReq * pUserInfoReq = (CMD_GR_UserInfoReq *)pData;
	WORD wCurDeskPos = pIServerUserItem->GetMobileUserDeskPos();
	WORD wMaxDeskPos = m_pGameServiceOption->wTableCount-pIServerUserItem->GetMobileUserDeskCount();

	//数量效验
	if(pUserInfoReq->wTablePos > wMaxDeskPos) pUserInfoReq->wTablePos = wMaxDeskPos;

	//更新信息
	pIServerUserItem->SetMobileUserDeskPos(pUserInfoReq->wTablePos);
	
	//发送信息
	SendViewTableUserInfoPacketToMobileUser(pIServerUserItem,pUserInfoReq->dwUserIDReq);

	return true;
}

//请求更换位置
bool CAttemperEngineSink::OnTCPNetworkSubUserChairReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//用户状态
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		//失败
		m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),REQUEST_FAILURE_NORMAL);
		return true;
	}

	//查找桌子
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//过滤同桌
		if(i == pIServerUserItem->GetTableID())continue;

		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//无效过滤
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;

		//离开处理
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
		}

		//用户坐下
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//查找同桌
	if(pIServerUserItem->GetTableID() != INVALID_TABLE)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
		if (pTableFrame->IsGameStarted()==false && pTableFrame->IsTableLocked()==false)
		{
			//无效过滤
			WORD wChairID=pTableFrame->GetRandNullChairID();
			if (wChairID!=INVALID_CHAIR)
			{
				//离开处理
				if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
				{
					CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
					if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
				}

				//用户坐下
				pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
				return true;
			}
		}
	}

	//失败
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("没找到可进入的游戏桌！"),REQUEST_FAILURE_NORMAL);
	return true;
}

//请求椅子用户信息
bool CAttemperEngineSink::OnTCPNetworkSubChairUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_ChairUserInfoReq));
	if (wDataSize!=sizeof(CMD_GR_ChairUserInfoReq)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_ChairUserInfoReq * pUserInfoReq = (CMD_GR_ChairUserInfoReq *)pData;
	if(pUserInfoReq->wTableID == INVALID_TABLE) return true;
	if(pUserInfoReq->wTableID >= m_pGameServiceOption->wTableCount)return true;

	//发送消息
	WORD wChairCout = m_TableFrameArray[pUserInfoReq->wTableID]->GetChairCount();
	for(WORD wIndex = 0; wIndex < wChairCout; wIndex++)
	{
		//获取用户
		if(pUserInfoReq->wChairID != INVALID_CHAIR && wIndex != pUserInfoReq->wChairID)continue;
		IServerUserItem * pTagerIServerUserItem=m_TableFrameArray[pUserInfoReq->wTableID]->GetTableUserItem(wIndex);
		if(pTagerIServerUserItem==NULL)continue;

		//变量定义
		BYTE cbBuffer[SOCKET_TCP_PACKET]={0};
		tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));
		tagUserInfo *pUserInfo = pTagerIServerUserItem->GetUserInfo();

		//用户属性
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//用户属性
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//用户状态
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//用户局数
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//用户成绩
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

		//叠加信息
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//发送消息
		WORD wHeadSize=sizeof(tagMobileUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//查询银行
bool CAttemperEngineSink::OnTCPNetworkSubQueryInsureInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_QueryInsureInfoRequest));
	if (wDataSize!=sizeof(CMD_GR_C_QueryInsureInfoRequest)) return false;

	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)||(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH));
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0&&(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_C_QueryInsureInfoRequest * pQueryInsureInfoRequest = (CMD_GR_C_QueryInsureInfoRequest *)pData;

	//变量定义
	DBR_GR_QueryInsureInfo QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//构造数据
	QueryInsureInfo.cbActivityGame=pQueryInsureInfoRequest->cbActivityGame;
	QueryInsureInfo.dwUserID=pIServerUserItem->GetUserID();
	QueryInsureInfo.dwClientAddr=pIServerUserItem->GetClientAddr();

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(QueryInsureInfo.dwUserID,DBR_GR_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return true;
}

//存款请求
bool CAttemperEngineSink::OnTCPNetworkSubSaveScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_SaveScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_SaveScoreRequest)) return false;

	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)||(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH));
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0&&(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//变量定义
	CMD_GR_C_SaveScoreRequest * pSaveScoreRequest=(CMD_GR_C_SaveScoreRequest *)pData;

	//效验参数
	ASSERT(pSaveScoreRequest->lSaveScore>0L);
	if (pSaveScoreRequest->lSaveScore<=0L) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//规则判断
	if(pSaveScoreRequest->cbActivityGame == FALSE && CServerRule::IsForfendSaveInRoom(m_pGameServiceOption->dwServerRule))
	{
		//发送数据
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止房间存款，存入操作失败！"),0L,pSaveScoreRequest->cbActivityGame);
		return true;
	}

	//规则判断
	if(pSaveScoreRequest->cbActivityGame == TRUE && CServerRule::IsForfendSaveInGame(m_pGameServiceOption->dwServerRule))
	{
		//发送数据
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止游戏存款，存入操作失败！"),0L,pSaveScoreRequest->cbActivityGame);
		return true;
	}

	//变量定义
	SCORE lConsumeQuota=0L;
	SCORE lUserWholeScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();

	//获取限额
	if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		lConsumeQuota=m_TableFrameArray[wTableID]->QueryConsumeQuota(pIServerUserItem);
	}
	else
	{
		lConsumeQuota=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	}

	//限额判断
	if (pSaveScoreRequest->lSaveScore>lConsumeQuota)
	{
		if (lConsumeQuota<lUserWholeScore)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("由于您正在游戏中，游戏币可存入额度为 %I64d，存入操作失败！"),lConsumeQuota);

			//发送数据
			SendInsureFailure(pIServerUserItem,szDescribe,0L,pSaveScoreRequest->cbActivityGame);
		}
		else
		{
			//发送数据
			SendInsureFailure(pIServerUserItem,TEXT("您的游戏币余额不足，存入操作失败！"),0L,pSaveScoreRequest->cbActivityGame);
		}

		return true;
	}

	//锁定积分
	if (pIServerUserItem->FrozenedUserScore(pSaveScoreRequest->lSaveScore)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	DBR_GR_UserSaveScore UserSaveScore;
	ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

	//构造数据
	UserSaveScore.cbActivityGame=pSaveScoreRequest->cbActivityGame;
	UserSaveScore.dwUserID=pIServerUserItem->GetUserID();
	UserSaveScore.lSaveScore=pSaveScoreRequest->lSaveScore;
	UserSaveScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserSaveScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserSaveScore.szMachineID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

	return true;
}

//取款请求
bool CAttemperEngineSink::OnTCPNetworkSubTakeScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_TakeScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_TakeScoreRequest)) return false;

	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)||(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH));
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0&&(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//变量定义
	CMD_GR_C_TakeScoreRequest * pTakeScoreRequest=(CMD_GR_C_TakeScoreRequest *)pData;
	pTakeScoreRequest->szInsurePass[CountArray(pTakeScoreRequest->szInsurePass)-1]=0;

	//效验参数
	ASSERT(pTakeScoreRequest->lTakeScore>0L);
	if (pTakeScoreRequest->lTakeScore<=0L) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//规则判断
	if(pTakeScoreRequest->cbActivityGame == FALSE && CServerRule::IsForfendTakeInRoom(m_pGameServiceOption->dwServerRule))
	{
		//发送数据
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止房间取款，取出操作失败！"),0L,pTakeScoreRequest->cbActivityGame);
		return true;
	}

	//规则判断
	if(pTakeScoreRequest->cbActivityGame == TRUE && CServerRule::IsForfendTakeInGame(m_pGameServiceOption->dwServerRule))
	{
		//发送数据
		SendInsureFailure(pIServerUserItem,TEXT("此房间禁止游戏取款，取出操作失败！"),0L,pTakeScoreRequest->cbActivityGame);
		return true;
	}

	//变量定义
	DBR_GR_UserTakeScore UserTakeScore;
	ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

	//构造数据
	UserTakeScore.cbActivityGame=pTakeScoreRequest->cbActivityGame;
	UserTakeScore.dwUserID=pIServerUserItem->GetUserID();
	UserTakeScore.lTakeScore=pTakeScoreRequest->lTakeScore;
	UserTakeScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserTakeScore.szPassword,pTakeScoreRequest->szInsurePass,CountArray(UserTakeScore.szPassword));
	lstrcpyn(UserTakeScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTakeScore.szMachineID));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

	return true;
}

//转账请求
bool CAttemperEngineSink::OnTCPNetworkSubTransferScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GP_C_TransferScoreRequest));
	if (wDataSize!=sizeof(CMD_GP_C_TransferScoreRequest)) return false;

	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)||(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH));
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0&&(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//变量定义
	CMD_GP_C_TransferScoreRequest * pTransferScoreRequest=(CMD_GP_C_TransferScoreRequest *)pData;
	pTransferScoreRequest->szNickName[CountArray(pTransferScoreRequest->szNickName)-1]=0;
	pTransferScoreRequest->szInsurePass[CountArray(pTransferScoreRequest->szInsurePass)-1]=0;

	//效验参数
	ASSERT(pTransferScoreRequest->lTransferScore>0L);
	if (pTransferScoreRequest->lTransferScore<=0L) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	DBR_GR_UserTransferScore UserTransferScore;
	ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

	//构造数据
	UserTransferScore.cbActivityGame=pTransferScoreRequest->cbActivityGame;
	UserTransferScore.dwUserID=pIServerUserItem->GetUserID();
	UserTransferScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	UserTransferScore.cbByNickName=pTransferScoreRequest->cbByNickName;
	UserTransferScore.lTransferScore=pTransferScoreRequest->lTransferScore;
	lstrcpyn(UserTransferScore.szNickName,pTransferScoreRequest->szNickName,CountArray(UserTransferScore.szNickName));
	lstrcpyn(UserTransferScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTransferScore.szMachineID));
	lstrcpyn(UserTransferScore.szPassword,pTransferScoreRequest->szInsurePass,CountArray(UserTransferScore.szPassword));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

	return true;
}

//查询用户请求
bool CAttemperEngineSink::OnTCPNetworkSubQueryUserInfoRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_QueryUserInfoRequest));
	if (wDataSize!=sizeof(CMD_GR_C_QueryUserInfoRequest)) return false;

	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//变量定义
	CMD_GR_C_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GR_C_QueryUserInfoRequest *)pData;
	pQueryUserInfoRequest->szNickName[CountArray(pQueryUserInfoRequest->szNickName)-1]=0;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	DBR_GR_QueryTransferUserInfo QueryTransferUserInfo;
	ZeroMemory(&QueryTransferUserInfo,sizeof(QueryTransferUserInfo));

	//构造数据
	QueryTransferUserInfo.cbActivityGame=pQueryUserInfoRequest->cbActivityGame;
	QueryTransferUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
	lstrcpyn(QueryTransferUserInfo.szNickName,pQueryUserInfoRequest->szNickName,CountArray(QueryTransferUserInfo.szNickName));
	QueryTransferUserInfo.dwUserID=pIServerUserItem->GetUserID();

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_QUERY_TRANSFER_USER_INFO,dwSocketID,&QueryTransferUserInfo,sizeof(QueryTransferUserInfo));

	return true;
}

//查询设置
bool CAttemperEngineSink::OnTCPNetworkSubQueryOption(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//变量定义
	CMD_GR_OptionCurrent OptionCurrent;
	ZeroMemory(&OptionCurrent,sizeof(OptionCurrent));

	//挂接属性
	OptionCurrent.ServerOptionInfo.wKindID=m_pGameServiceOption->wKindID;
	OptionCurrent.ServerOptionInfo.wNodeID=m_pGameServiceOption->wNodeID;
	OptionCurrent.ServerOptionInfo.wSortID=m_pGameServiceOption->wSortID;

	//房间配置
	OptionCurrent.ServerOptionInfo.wRevenueRatio=m_pGameServiceOption->wRevenueRatio;
	OptionCurrent.ServerOptionInfo.lServiceScore=m_pGameServiceOption->lServiceScore;
	OptionCurrent.ServerOptionInfo.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	OptionCurrent.ServerOptionInfo.lMinTableScore=m_pGameServiceOption->lMinTableScore;
	OptionCurrent.ServerOptionInfo.lMinEnterScore=m_pGameServiceOption->lMinEnterScore;
	OptionCurrent.ServerOptionInfo.lMaxEnterScore=m_pGameServiceOption->lMaxEnterScore;

	//会员限制
	OptionCurrent.ServerOptionInfo.cbMinEnterMember=m_pGameServiceOption->cbMinEnterMember;
	OptionCurrent.ServerOptionInfo.cbMaxEnterMember=m_pGameServiceOption->cbMaxEnterMember;

	//房间属性
	OptionCurrent.ServerOptionInfo.dwServerRule=m_pGameServiceOption->dwServerRule;
	lstrcpyn(OptionCurrent.ServerOptionInfo.szServerName,m_pGameServiceOption->szServerName,CountArray(OptionCurrent.ServerOptionInfo.szServerName));

	//聊天规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_ON_GAME;

	//房间规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_LOOKON;

	//银行规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_GAME;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_GAME;

	//其他规则
	//OptionCurrent.dwRuleMask|=SR_RECORD_GAME_TRACK;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_RULE;
	OptionCurrent.dwRuleMask|=SR_FORFEND_LOCK_TABLE;
	OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_SIMULATE;

	//组件规则
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_DYNAMIC_JOIN;
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_ATTEND;
	//if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_OFFLINE_TRUSTEE;

	//模式规则
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0) OptionCurrent.dwRuleMask|=SR_RECORD_GAME_SCORE;
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0) OptionCurrent.dwRuleMask|=SR_IMMEDIATE_WRITE_SCORE;

	//发送数据
	SendData(pIServerUserItem,MDM_GR_MANAGE,SUB_GR_OPTION_CURRENT,&OptionCurrent,sizeof(OptionCurrent));

	return true;
}

//房间设置
bool CAttemperEngineSink::OnTCPNetworkSubOptionServer(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_ServerOption));
	if (wDataSize!=sizeof(CMD_GR_ServerOption)) return false;

	//变量定义
	CMD_GR_ServerOption * pServerOption=(CMD_GR_ServerOption *)pData;
	tagServerOptionInfo * pServerOptionInfo=&pServerOption->ServerOptionInfo;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//变量定义
	bool bModifyServer=false;

	//挂接节点
	if (m_pGameServiceOption->wNodeID!=pServerOptionInfo->wNodeID)
	{
		bModifyServer=true;
		m_pGameServiceOption->wNodeID=pServerOptionInfo->wNodeID;
	}

	//挂接类型
	if ((pServerOptionInfo->wKindID!=0)&&(m_pGameServiceOption->wKindID!=pServerOptionInfo->wKindID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wKindID=pServerOptionInfo->wKindID;
	}
	
	//挂接排序
	if ((pServerOptionInfo->wSortID!=0)&&(m_pGameServiceOption->wSortID!=pServerOptionInfo->wSortID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wSortID=pServerOptionInfo->wSortID;
	}

	//房间名字
	if ((pServerOptionInfo->szServerName[0]!=0)&&(lstrcmp(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName)!=0))
	{
		bModifyServer=true;
		lstrcpyn(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName,CountArray(m_pGameServiceOption->szServerName));
	}

	//税收配置
	m_pGameServiceOption->wRevenueRatio=pServerOptionInfo->wRevenueRatio;
	m_pGameServiceOption->lServiceScore=pServerOptionInfo->lServiceScore;

	//房间配置
	m_pGameServiceOption->lRestrictScore=pServerOptionInfo->lRestrictScore;
	m_pGameServiceOption->lMinTableScore=pServerOptionInfo->lMinTableScore;
	m_pGameServiceOption->lMinEnterScore=pServerOptionInfo->lMinEnterScore;
	m_pGameServiceOption->lMaxEnterScore=pServerOptionInfo->lMaxEnterScore;

	//会员限制
	m_pGameServiceOption->cbMinEnterMember=pServerOptionInfo->cbMinEnterMember;
	m_pGameServiceOption->cbMaxEnterMember=pServerOptionInfo->cbMaxEnterMember;

	//聊天规则
	CServerRule::SetForfendGameChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendRoomChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperOnGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperOnGame(pServerOptionInfo->dwServerRule));

	//房间规则
	CServerRule::SetForfendRoomEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameLookon(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameLookon(pServerOptionInfo->dwServerRule));

	//银行规则
	CServerRule::SetForfendTakeInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendTakeInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInGame(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInGame(pServerOptionInfo->dwServerRule));

	//其他规则
	CServerRule::SetRecordGameTrack(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameTrack(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameRule(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameRule(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendLockTable(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendLockTable(pServerOptionInfo->dwServerRule));

	//动态加入
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE)
	{
		CServerRule::SetAllowDynamicJoin(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowDynamicJoin(pServerOptionInfo->dwServerRule));
	}

	//机器管理
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE)
	{
		CServerRule::SetAllowAndroidAttend(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowAndroidAttend(pServerOptionInfo->dwServerRule));
	}

	//断线托管
	if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE)
	{
		CServerRule::SetAllowOffLineTrustee(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowOffLineTrustee(pServerOptionInfo->dwServerRule));
	}

	//记录成绩
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0)
	{
		CServerRule::SetRecordGameScore(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameScore(pServerOptionInfo->dwServerRule));
	}

	//立即写分
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0)
	{
		CServerRule::SetImmediateWriteScore(m_pGameServiceOption->dwServerRule,CServerRule::IsImmediateWriteScore(pServerOptionInfo->dwServerRule));
	}

	//调整参数
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->RectifyServiceParameter();

	//发送修改
	if (bModifyServer==true)
	{
		//变量定义
		CMD_CS_C_ServerModify ServerModify;
		ZeroMemory(&ServerModify,sizeof(ServerModify));

		//服务端口
		ServerModify.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();

		//房间信息
		ServerModify.wKindID=m_pGameServiceOption->wKindID;
		ServerModify.wNodeID=m_pGameServiceOption->wNodeID;
		ServerModify.wSortID=m_pGameServiceOption->wSortID;
		ServerModify.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		ServerModify.dwFullCount=m_pGameServiceOption->wMaxPlayer-RESERVE_USER_COUNT;
		lstrcpyn(ServerModify.szServerName,m_pGameServiceOption->szServerName,CountArray(ServerModify.szServerName));
		lstrcpyn(ServerModify.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(ServerModify.szServerAddr));

		//发送数据
		m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_MODIFY,&ServerModify,sizeof(ServerModify));
	}

	//发送信息
	SendRoomMessage(pIServerUserItem,TEXT("当前游戏服务器房间的“运行值”状态配置数据修改成功"),SMT_CHAT|SMT_EJECT);

	//输出信息
	TCHAR szBuffer[128]=TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("远程修改房间配置通知 管理员 %s [ %ld ]"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserID());

	//输出信息
	CTraceService::TraceString(szBuffer,TraceLevel_Info);

	return true;
}

//踢出用户
bool CAttemperEngineSink::OnTCPNetworkSubManagerKickUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_KickUser));
	if (wDataSize!=sizeof(CMD_GR_KickUser)) return false;

	//变量定义
	CMD_GR_KickUser * pKickUser=(CMD_GR_KickUser *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//权限判断
	ASSERT(CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==false) return false;

	//目标用户
	IServerUserItem * pITargetUserItem = m_ServerUserManager.SearchUserItem(pKickUser->dwTargetUserID);
	if(pITargetUserItem==NULL) return true;

	////用户状态
	//if(pITargetUserItem->GetUserStatus()==US_PLAYING) return true;

	//踢出记录
	m_KickUserItemMap[pITargetUserItem->GetUserID()]=(DWORD)time(NULL);

	//请离桌子
	WORD wTargerTableID = pITargetUserItem->GetTableID();
	if(wTargerTableID != INVALID_TABLE)
	{
		//发送消息
		SendGameMessage(pITargetUserItem,TEXT("你已被管理员请离桌子！"),SMT_CHAT|SMT_CLOSE_GAME);

		CTableFrame * pTableFrame=m_TableFrameArray[wTargerTableID];
		if (pTableFrame->PerformStandUpAction(pITargetUserItem)==false) return true;
	}

	//发送通知
	LPCTSTR pszMessage=TEXT("你已被管理员请离此游戏房间！");
	SendRoomMessage(pITargetUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

	pITargetUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);

	return true;
}

//限制聊天
bool CAttemperEngineSink::OnTCPNetworkSubLimitUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_GR_LimitUserChat));
	if (wDataSize!=sizeof(CMD_GR_LimitUserChat)) return false;

	//消息处理
	CMD_GR_LimitUserChat * pLimitUserChat=(CMD_GR_LimitUserChat *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//权限判断
	//ASSERT(CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==true);
	//if (CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==false) return false;

	//目标用户
	IServerUserItem * pITargerUserItem=m_ServerUserManager.SearchUserItem(pLimitUserChat->dwTargetUserID);
	if (pITargerUserItem==NULL) return true;

	//变量定义
	DWORD dwAddRight = 0, dwRemoveRight = 0;

	//大厅聊天
	if (pLimitUserChat->cbLimitFlags==OSF_ROOM_CHAT)
	{
		if (CMasterRight::CanLimitRoomChat(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_ROOM_CHAT;
		else
			dwRemoveRight |= UR_CANNOT_ROOM_CHAT;
	}

	//游戏聊天
	if (pLimitUserChat->cbLimitFlags==OSF_GAME_CHAT)
	{
		if (CMasterRight::CanLimitGameChat(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_GAME_CHAT;
		else
			dwRemoveRight |= UR_CANNOT_GAME_CHAT;
	}

	//大厅私聊
	if (pLimitUserChat->cbLimitFlags==OSF_ROOM_WISPER)
	{
		if (CMasterRight::CanLimitWisper(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_WISPER;
		else
			dwRemoveRight |= UR_CANNOT_WISPER;
	}

	//发送喇叭
	if(pLimitUserChat->cbLimitFlags==OSF_SEND_BUGLE)
	{
		if (CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==false) return false;

		if(pLimitUserChat->cbLimitValue == TRUE)
			dwAddRight |= UR_CANNOT_BUGLE;
		else
			dwRemoveRight |= UR_CANNOT_BUGLE;
	}

	if( dwAddRight != 0 || dwRemoveRight != 0 )
	{
		pITargerUserItem->ModifyUserRight(dwAddRight,dwRemoveRight);

		//发送通知
		CMD_GR_ConfigUserRight cur = {0};
		cur.dwUserRight = pITargerUserItem->GetUserRight();

		SendData( pITargerUserItem,MDM_GR_CONFIG,SUB_GR_CONFIG_USER_RIGHT,&cur,sizeof(cur) );

		//发送消息
		SendRoomMessage(pIServerUserItem,TEXT("用户聊天权限配置成功！"),SMT_CHAT);
	}
	else return false;

	return true;
}

//踢出所有用户
bool CAttemperEngineSink::OnTCPNetworkSubKickAllUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//消息处理
	CMD_GR_KickAllUser * pKillAllUser=(CMD_GR_KickAllUser *)pData;

	//效验数据
	ASSERT(wDataSize<=sizeof(CMD_GR_KickAllUser));
	if( wDataSize > sizeof(CMD_GR_KickAllUser) ) return false;
	ASSERT(wDataSize==CountStringBuffer(pKillAllUser->szKickMessage));
	if (wDataSize!=CountStringBuffer(pKillAllUser->szKickMessage)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//权限判断
	ASSERT(CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==false) return false;

	//解散所有游戏
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if ( !pTableFrame->IsGameStarted() ) continue;

		pTableFrame->DismissGame();
	}

	tagBindParameter *pBindParameter = m_pNormalParameter;
	for( INT i = 0; i < m_pGameServiceOption->wMaxPlayer; i++ )
	{
		//目录用户
		IServerUserItem * pITargerUserItem= pBindParameter->pIServerUserItem;
		if (pITargerUserItem==NULL || pITargerUserItem==pIServerUserItem ) 
		{
			pBindParameter++;
			continue;
		}

		//发送消息
		SendRoomMessage(pITargerUserItem,pKillAllUser->szKickMessage,SMT_CHAT|SMT_EJECT|SMT_CLOSE_LINK|SMT_CLOSE_ROOM);

		pBindParameter++;
	} 

	return true;
}

//发布消息
bool CAttemperEngineSink::OnTCPNetworkSubSendMessage(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//权限判断
	ASSERT(CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==false) return false;

	//消息处理
	return SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);
}

//解散游戏
bool CAttemperEngineSink::OnTCPNetworkSubDismissGame(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_GR_DismissGame));
	if (wDataSize!=sizeof(CMD_GR_DismissGame)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//权限判断
	ASSERT(CMasterRight::CanDismissGame(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanDismissGame(pIServerUserItem->GetMasterRight())==false) return false;

	//消息处理
	CMD_GR_DismissGame * pDismissGame=(CMD_GR_DismissGame *)pData;

	//效验数据
	if(pDismissGame->wDismissTableNum >= m_TableFrameArray.GetCount()) return true;

	//解散游戏
	CTableFrame *pTableFrame=m_TableFrameArray[pDismissGame->wDismissTableNum];
	if(pTableFrame)
	{
		if(pTableFrame->IsGameStarted()) pTableFrame->DismissGame();
	}

	return true;
}

//用户登录
VOID CAttemperEngineSink::OnEventUserLogon(IServerUserItem * pIServerUserItem, bool bAlreadyOnLine)
{
	//获取参数
	WORD wBindIndex=pIServerUserItem->GetBindIndex();
	bool bAndroidUser=pIServerUserItem->IsAndroidUser();
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//登录处理
	if (pIServerUserItem->IsMobileUser()==false)
	{
		//变量定义
		CMD_GR_LogonSuccess LogonSuccess;
		//CMD_GR_ConfigServer ConfigServer;
		ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));
		//ZeroMemory(&ConfigServer,sizeof(ConfigServer));

		//登录成功
		LogonSuccess.dwUserRight=pIServerUserItem->GetUserRight();
		LogonSuccess.dwMasterRight=pIServerUserItem->GetMasterRight();
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

		//房间配置
		//--------
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)cbDataBuffer;
		ZeroMemory(pConfigServer,sizeof(CMD_GR_ConfigServer));

		WORD wHeadSize1=sizeof(CMD_GR_ConfigServer);
		CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize1,sizeof(cbDataBuffer)-wHeadSize1);

		pConfigServer->wTableCount=m_pGameServiceOption->wTableCount;
		pConfigServer->wChairCount=m_pGameServiceAttrib->wChairCount;
		pConfigServer->wServerType=m_pGameServiceOption->wServerType;
		pConfigServer->dwServerRule=m_pGameServiceOption->dwServerRule;

		//假桌子信息
		DTP_GP_FakeTableInfo FakeTableInfo = {0};

		FakeTableInfo.wChairID = 0;
		FakeTableInfo.dwUserID = 10001;
		sprintf(FakeTableInfo.szNickName, "uid%d", FakeTableInfo.dwUserID);
		SendPacket.AddPacket(&FakeTableInfo, sizeof(DTP_GP_FakeTableInfo), DTP_GP_FAKE_TABLE_INFO);

		FakeTableInfo.wChairID = 1;
		FakeTableInfo.dwUserID = 10002;
		sprintf(FakeTableInfo.szNickName, "uid%d", FakeTableInfo.dwUserID);
		SendPacket.AddPacket(&FakeTableInfo, sizeof(DTP_GP_FakeTableInfo), DTP_GP_FAKE_TABLE_INFO);

		FakeTableInfo.wChairID = 2;
		FakeTableInfo.dwUserID = 10003;
		sprintf(FakeTableInfo.szNickName, "uid%d", FakeTableInfo.dwUserID);
		SendPacket.AddPacket(&FakeTableInfo, sizeof(DTP_GP_FakeTableInfo), DTP_GP_FAKE_TABLE_INFO);

		FakeTableInfo.wChairID = 3;
		FakeTableInfo.dwUserID = 10004;
		sprintf(FakeTableInfo.szNickName, "uid%d", FakeTableInfo.dwUserID);
		SendPacket.AddPacket(&FakeTableInfo, sizeof(DTP_GP_FakeTableInfo), DTP_GP_FAKE_TABLE_INFO);

		FakeTableInfo.wChairID = 4;
		FakeTableInfo.dwUserID = 10005;
		sprintf(FakeTableInfo.szNickName, "uid%d", FakeTableInfo.dwUserID);
		SendPacket.AddPacket(&FakeTableInfo, sizeof(DTP_GP_FakeTableInfo), DTP_GP_FAKE_TABLE_INFO);

		FakeTableInfo.wChairID = 5;
		FakeTableInfo.dwUserID = 10006;
		sprintf(FakeTableInfo.szNickName, "uid%d", FakeTableInfo.dwUserID);
		SendPacket.AddPacket(&FakeTableInfo, sizeof(DTP_GP_FakeTableInfo), DTP_GP_FAKE_TABLE_INFO);

		WORD wSendSize1=SendPacket.GetDataSize()+sizeof(CMD_GR_ConfigServer);
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_SERVER,cbDataBuffer,wSendSize1);
		//--------

		//列表配置
		WORD wConfigColumnHead=sizeof(m_DataConfigColumn)-sizeof(m_DataConfigColumn.ColumnItem);
		WORD wConfigColumnInfo=m_DataConfigColumn.cbColumnCount*sizeof(m_DataConfigColumn.ColumnItem[0]);
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_COLUMN,&m_DataConfigColumn,wConfigColumnHead+wConfigColumnInfo);

		//道具配置
		WORD wConfigPropertyHead=sizeof(m_DataConfigProperty)-sizeof(m_DataConfigProperty.PropertyInfo);
		WORD wConfigPropertyInfo=m_DataConfigProperty.cbPropertyCount*sizeof(m_DataConfigProperty.PropertyInfo[0]);
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_PROPERTY,&m_DataConfigProperty,wConfigPropertyHead+wConfigPropertyInfo);

		//配置完成
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_FINISH,NULL,0);

		//玩家数据
		SendUserInfoPacket(pIServerUserItem,pBindParameter->dwSocketID);

		//在线用户
		WORD wUserIndex=0;
		IServerUserItem * pIServerUserItemSend=NULL;
		while (true)
		{
			pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
			if (pIServerUserItemSend==NULL) break;
			if (pIServerUserItemSend==pIServerUserItem) continue;
			SendUserInfoPacket(pIServerUserItemSend,pBindParameter->dwSocketID);
		}

		//桌子状态
		CMD_GR_TableInfo TableInfo;
		TableInfo.wTableCount=(WORD)m_TableFrameArray.GetCount();
		ASSERT(TableInfo.wTableCount<CountArray(TableInfo.TableStatusArray));
		for (WORD i=0;i<TableInfo.wTableCount;i++)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[i];
			TableInfo.TableStatusArray[i].cbTableLock=pTableFrame->IsTableLocked()?TRUE:FALSE;
			TableInfo.TableStatusArray[i].cbPlayStatus=pTableFrame->IsTableStarted()?TRUE:FALSE;
		}

		//桌子状态
		WORD wHeadSize=sizeof(TableInfo)-sizeof(TableInfo.TableStatusArray);
		WORD wSendSize=wHeadSize+TableInfo.wTableCount*sizeof(TableInfo.TableStatusArray[0]);
		SendData(pBindParameter->dwSocketID,MDM_GR_STATUS,SUB_GR_TABLE_INFO,&TableInfo,wSendSize);

		//发送通知
		if (bAlreadyOnLine==false)
		{
			SendUserInfoPacket(pIServerUserItem,INVALID_DWORD);
		}

		//登录完成
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);

		//欢迎消息
		if (bAndroidUser==false)
		{
			////构造提示
			//TCHAR szMessage[128]=TEXT("");
			//_sntprintf(szMessage,CountArray(szMessage),TEXT("欢迎您进入“%s”游戏房间，祝您游戏愉快！"),m_pGameServiceAttrib->szGameName);

			////发送消息
			//SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);

			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("亲爱的用户，欢迎您进入669休闲游戏平台！\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("\n抵制不良游戏，拒绝盗版游戏。 \n注意自我保护，谨防受骗上当。 \n适度游戏益脑，沉迷游戏伤身。 \n合理安排时间，享受健康生活。\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("加强防范，防止盗号，严禁利用游戏平台进行赌博行为，构建和谐网络环境。\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("手机版捕鱼抢先登陆安卓系统手机，平板，苹果系统手机，平板。"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("支持电脑手机同桌游戏，随时随地畅享669捕鱼，快来体验吧！\n手机版下载地址：m.45669.com\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("1.2喜从天降礼品版盛大开启，话费，平板，手机，样样有。"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("无论您是手机端玩家还是电脑端玩家，游戏中击中任何鱼都有机会获得由本平台赠送的话费，平板，手机。"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("所在游戏房间倍率越高，奖品越丰厚哦。\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
		}
	}
	else
	{
		//变量定义
		CMD_GR_ConfigServer ConfigServer;
		ZeroMemory(&ConfigServer,sizeof(ConfigServer));

		//房间配置
		ConfigServer.wTableCount=m_pGameServiceOption->wTableCount;
		ConfigServer.wChairCount=m_pGameServiceAttrib->wChairCount;
		ConfigServer.wServerType=m_pGameServiceOption->wServerType;
		ConfigServer.dwServerRule=m_pGameServiceOption->dwServerRule;
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_SERVER,&ConfigServer,sizeof(ConfigServer));

		//配置完成
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_FINISH,NULL,0);

		//玩家数据
		SendViewTableUserInfoPacketToMobileUser(pIServerUserItem,pIServerUserItem->GetUserID());

		//群发用户
		SendUserInfoPacket(pIServerUserItem,INVALID_DWORD);

		//登录完成
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);

		//立即登录
		if(pIServerUserItem->GetTableID()==INVALID_TABLE)
		{
			WORD wMobileUserRule =  pIServerUserItem->GetMobileUserRule();
			if((wMobileUserRule&BEHAVIOR_LOGON_IMMEDIATELY)!=0)
			{
				MobileUserImmediately(pIServerUserItem);
			}
			else
			{
				SendViewTableUserInfoPacketToMobileUser(pIServerUserItem,INVALID_CHAIR);
			}
		}
	}

	//网络设置
	if (bAndroidUser==false)
	{
		if (pBindParameter->cbClientKind==CLIENT_KIND_MOBILE)
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_MOBILE);
		}
		else
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_COMPUTER);
		}
	}

	if(m_pIGameMatchServiceManager!=NULL)
	{
		m_pIGameMatchServiceManager->SendMatchInfo(pIServerUserItem);
	}
	return;
}

//用户离开
VOID CAttemperEngineSink::OnEventUserLogout(IServerUserItem * pIServerUserItem, DWORD dwLeaveReason)
{
	//变量定义
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//提取成绩
	pIServerUserItem->QueryRecordInfo(LeaveGameServer.RecordInfo);
	pIServerUserItem->DistillVariation(LeaveGameServer.VariationInfo);

	//用户信息
	LeaveGameServer.dwLeaveReason=dwLeaveReason;
	LeaveGameServer.dwUserID=pIServerUserItem->GetUserID();
	LeaveGameServer.dwInoutIndex=pIServerUserItem->GetInoutIndex();
	LeaveGameServer.dwOnLineTimeCount=(DWORD)(time(NULL))-pIServerUserItem->GetLogonTime();

	//连接信息
	LeaveGameServer.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(LeaveGameServer.szMachineID,pIServerUserItem->GetMachineID(),CountArray(LeaveGameServer.szMachineID));
	lstrcpyn(LeaveGameServer.szNickName,pIServerUserItem->GetNickName(),CountArray(LeaveGameServer.szNickName));
	lstrcpyn(LeaveGameServer.szServerName,m_pGameServiceOption->szServerName,CountArray(LeaveGameServer.szServerName));

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer), TRUE);
	
	//汇总用户
	if (m_bCollectUser==true)
	{
		//变量定义
		CMD_CS_C_UserLeave UserLeave;
		ZeroMemory(&UserLeave,sizeof(UserLeave));

		//设置变量
		UserLeave.dwUserID=pIServerUserItem->GetUserID();

		//发送消息
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_LEAVE,&UserLeave,sizeof(UserLeave));
	}

	//知道比赛服务退出游戏
	if(m_pIGameMatchServiceManager!=NULL)m_pIGameMatchServiceManager->OnUserQuitGame(pIServerUserItem, 0);

	//分组删除
	DeleteWaitDistribute(pIServerUserItem);

	//删除用户
	m_ServerUserManager.DeleteUserItem(pIServerUserItem);

	return;
}

//解锁游戏币
bool CAttemperEngineSink::PerformUnlockScore(DWORD dwUserID, DWORD dwInoutIndex, DWORD dwLeaveReason)
{
	//变量定义
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//设置变量
	LeaveGameServer.dwUserID=dwUserID;
	LeaveGameServer.dwInoutIndex=dwInoutIndex;
	LeaveGameServer.dwLeaveReason=dwLeaveReason;

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(dwUserID,DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer));

	return true;
}

//版本检查
bool CAttemperEngineSink::PerformCheckVersion(DWORD dwPlazaVersion, DWORD dwFrameVersion, DWORD dwClientVersion, DWORD dwSocketID)
{
	//变量定义
	bool bMustUpdateClient=false;
	bool bAdviceUpdateClient=false;

	//游戏版本
	if (VERSION_EFFICACY==TRUE)
	{
		if (GetSubVer(dwClientVersion)<GetSubVer(m_pGameServiceAttrib->dwClientVersion)) bAdviceUpdateClient=true;
		if (GetMainVer(dwClientVersion)!=GetMainVer(m_pGameServiceAttrib->dwClientVersion)) bMustUpdateClient=true;
		if (GetProductVer(dwClientVersion)!=GetProductVer(m_pGameServiceAttrib->dwClientVersion)) bMustUpdateClient=true;
	}
	else
	{
		if (GetSubVer(dwClientVersion)<GetSubVer(m_pGameParameter->dwClientVersion)) bAdviceUpdateClient=true;
		if (GetMainVer(dwClientVersion)!=GetMainVer(m_pGameParameter->dwClientVersion)) bMustUpdateClient=true;
		if (GetProductVer(dwClientVersion)!=GetProductVer(m_pGameParameter->dwClientVersion)) bMustUpdateClient=true;
	}

	//更新通知
	if ((bMustUpdateClient==true)||(bAdviceUpdateClient==true))
	{
		//变量定义
		CMD_GR_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//变量定义
		UpdateNotify.cbMustUpdatePlaza=false;
		UpdateNotify.cbMustUpdateClient=bMustUpdateClient;
		UpdateNotify.cbAdviceUpdateClient=bAdviceUpdateClient;

		//当前版本
		UpdateNotify.dwCurrentPlazaVersion=VERSION_PLAZA;
		UpdateNotify.dwCurrentFrameVersion=VERSION_FRAME;
		UpdateNotify.dwCurrentClientVersion=m_pGameServiceAttrib->dwClientVersion;

		//发送消息
		SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//中止判断
		if (bMustUpdateClient==true)
		{
			printf("ShutDownSocket %d\n", __LINE__);
			//m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return true;
		}
	}

	return true;
}

//切换连接
bool CAttemperEngineSink::SwitchUserItemConnect(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	//效验参数
	ASSERT((pIServerUserItem!=NULL)&&(wTargetIndex!=INVALID_WORD));
	if ((pIServerUserItem==NULL)||(wTargetIndex==INVALID_WORD)) return false;

	//断开用户
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//发送通知
		LPCTSTR pszMessage=TEXT("请注意，您的帐号在另一地方进入了此游戏房间，您被迫离开！");
		SendRoomMessage(pIServerUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

		//绑定参数
		WORD wSourceIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pSourceParameter=GetBindParameter(wSourceIndex);

		if (pSourceParameter == NULL) return false;

		//解除绑定
		ASSERT((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem));
		if ((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem)) pSourceParameter->pIServerUserItem=NULL;

		//断开用户
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			m_AndroidUserManager.DeleteAndroidUserItem(pSourceParameter->dwSocketID);
		}
		else
		{
			printf("ShutDownSocket %d\n", __LINE__);
			m_pITCPNetworkEngine->ShutDownSocket(pSourceParameter->dwSocketID);
		}
	}

	//状态切换
	bool bIsOffLine=false;
	if (pIServerUserItem->GetUserStatus()==US_OFFLINE)
	{
		//变量定义
		WORD wTableID=pIServerUserItem->GetTableID();
		WORD wChairID=pIServerUserItem->GetChairID();

		//设置状态
		bIsOffLine=true;
		pIServerUserItem->SetUserStatus(US_PLAYING,wTableID,wChairID);
	}

	//机器判断
	LPCTSTR pszMachineID=pIServerUserItem->GetMachineID();
	bool bSameMachineID=(lstrcmp(pszMachineID,szMachineID)==0);

	//变量定义
	bool bAndroidUser=(wTargetIndex>=INDEX_ANDROID);
	tagBindParameter * pTargetParameter=GetBindParameter(wTargetIndex);

	//激活用户
	pTargetParameter->pIServerUserItem=pIServerUserItem;
	pIServerUserItem->SetUserParameter(pTargetParameter->dwClientAddr,wTargetIndex,szMachineID,bAndroidUser,false);

	//手机标识
	if(pTargetParameter->cbClientKind==CLIENT_KIND_MOBILE)
	{
		pIServerUserItem->SetMobileUser(true);
		SetMobileUserParameter(pIServerUserItem,cbDeviceType,wBehaviorFlags,wPageTableCount);
	}

	//登录事件
	OnEventUserLogon(pIServerUserItem,true);

	//安全提示
	if ((bAndroidUser==false)&&(bIsOffLine==false)&&(bSameMachineID==false))
	{
		SendRoomMessage(pIServerUserItem,TEXT("请注意，您的帐号在另一地方进入了此游戏房间，对方被迫离开！"),SMT_EJECT|SMT_CHAT|SMT_GLOBAL);
	}

	return true;
}

//登录失败
bool CAttemperEngineSink::SendLogonFailure(LPCTSTR pszString, LONG lErrorCode, DWORD dwSocketID)
{
	//变量定义
	CMD_GR_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));

	//构造数据
	LogonFailure.lErrorCode=lErrorCode;
	lstrcpyn(LogonFailure.szDescribeString,pszString,CountArray(LogonFailure.szDescribeString));

	//数据属性
	WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);

	//发送数据
	SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FAILURE,&LogonFailure,wHeadSize+wDataSize);

	return true;
}

//银行失败
bool CAttemperEngineSink::SendInsureFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszString, LONG lErrorCode,BYTE cbActivityGame)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_S_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//构造数据
	UserInsureFailure.cbActivityGame=cbActivityGame;
	UserInsureFailure.lErrorCode=lErrorCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pszString,CountArray(UserInsureFailure.szDescribeString));

	//数据属性
	WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);

	//发送数据
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDataSize);

	return true;
}

//请求失败
bool CAttemperEngineSink::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode)
{
	//变量定义
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//设置变量
	RequestFailure.lErrorCode=lErrorCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//发送数据
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//道具失败
bool CAttemperEngineSink::SendPropertyFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode,WORD wRequestArea)
{
	//变量定义
	CMD_GR_PropertyFailure PropertyFailure;
	ZeroMemory(&PropertyFailure,sizeof(PropertyFailure));

	//设置变量
	PropertyFailure.lErrorCode=lErrorCode;
	PropertyFailure.wRequestArea=wRequestArea;
	lstrcpyn(PropertyFailure.szDescribeString,pszDescribe,CountArray(PropertyFailure.szDescribeString));

	//发送数据
	WORD wDataSize=CountStringBuffer(PropertyFailure.szDescribeString);
	WORD wHeadSize=sizeof(PropertyFailure)-sizeof(PropertyFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_PROPERTY_FAILURE,&PropertyFailure,wHeadSize+wDataSize);

	return true;
}

//发送用户
bool CAttemperEngineSink::SendUserInfoPacket(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));

	//用户属性
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwGroupID=pUserInfo->dwGroupID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//用户属性
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserInfo->cbMasterOrder;

	//用户状态
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//用户局数
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwUserMedal=pUserInfo->dwUserMedal;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;
	pUserInfoHead->lLoveLiness=pUserInfo->lLoveLiness;

	//用户积分
	pUserInfoHead->lGrade=pUserInfo->lGrade;
	pUserInfoHead->lInsure=pUserInfo->lInsure;

	//用户成绩
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
	pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

	//叠加信息
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);
	SendPacket.AddPacket(pUserInfo->szGroupName,DTP_GR_GROUP_NAME);
	SendPacket.AddPacket(pUserInfo->szUnderWrite,DTP_GR_UNDER_WRITE);

	//发送数据
	if (dwSocketID==INVALID_DWORD)
	{
		WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
		SendUserInfoPacketBatchToMobileUser(pIServerUserItem);
	}
	else
	{
		WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(dwSocketID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//广播道具
bool CAttemperEngineSink::SendPropertyMessage(DWORD dwSourceID,DWORD dwTargerID,WORD wPropertyIndex,WORD wPropertyCount)
{
	//构造结构
	CMD_GR_S_PropertyMessage  PropertyMessage;
	PropertyMessage.wPropertyIndex=wPropertyIndex;
	PropertyMessage.dwSourceUserID=dwSourceID;
	PropertyMessage.dwTargerUserID=dwTargerID;
	PropertyMessage.wPropertyCount=wPropertyCount;

	//在线用户
	WORD wUserIndex=0;
	IServerUserItem * pIServerUserItemSend=NULL;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
		if (pIServerUserItemSend==NULL) break;
		SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	}

	return true;
}

//道具效应
bool CAttemperEngineSink::SendPropertyEffect(IServerUserItem * pIServerUserItem)
{
	//参数校验
	if(pIServerUserItem==NULL) return false;

	//构造结构
	CMD_GR_S_PropertyEffect  PropertyEffect;
	PropertyEffect.wUserID =pIServerUserItem->GetUserID();
	PropertyEffect.cbMemberOrder=pIServerUserItem->GetMemberOrder();

	//在线用户
	WORD wUserIndex=0;
	IServerUserItem * pIServerUserItemSend=NULL;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
		if (pIServerUserItemSend==NULL) break;
		SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_PROPERTY_EFFECT,&PropertyEffect,sizeof(PropertyEffect));
	}

	return true;
}

//道具事件
bool CAttemperEngineSink::OnEventPropertyBuyPrep(WORD cbRequestArea,WORD wPropertyIndex,IServerUserItem *pISourceUserItem,IServerUserItem *pTargetUserItem)
{
	//目标玩家
	if ( pTargetUserItem == NULL )
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("赠送失败，您要赠送的玩家已经离开！"), 0L,cbRequestArea);

		return false;
	}

	//房间判断
	if ( (m_pGameServiceOption->wServerType == GAME_GENRE_GOLD || m_pGameServiceOption->wServerType == GAME_GENRE_MATCH ) && 
		(wPropertyIndex== PROPERTY_ID_SCORE_CLEAR||wPropertyIndex==PROPERTY_ID_TWO_CARD||wPropertyIndex == PROPERTY_ID_FOUR_CARD||wPropertyIndex == PROPERTY_ID_POSSESS) )
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("此房间不可以使用此道具,购买失败"), 0L,cbRequestArea);

		return false;
	}

	//查找道具
	tagPropertyInfo * pPropertyInfo=m_GamePropertyManager.SearchPropertyItem(wPropertyIndex);

	//有效效验
	if(pPropertyInfo==NULL)
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("此道具还未启用,购买失败！"), 0L,cbRequestArea);

		return false;
	}

	//自己使用
    if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_MESELF)==0 && pISourceUserItem==pTargetUserItem) 
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("此道具不可自己使用,购买失败！"), 0L,cbRequestArea);

		return false;
	}

	//玩家使用
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_PLAYER)==0 && pISourceUserItem!=pTargetUserItem) 
	{
		//发送消息
		SendPropertyFailure(pISourceUserItem,TEXT("此道具不可赠送给玩家,只能自己使用,购买失败！"), 0L,cbRequestArea);

		return false;
	}

	//旁观范围
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_LOOKON)==0)  
	{
		//变量定义
		WORD wTableID = pTargetUserItem->GetTableID();
		if(wTableID!=INVALID_TABLE)
		{
			//变量定义
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//获取桌子
			CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	
			//枚举用户
			do
			{
				//获取用户
				pIServerUserItem=pTableFrame->EnumLookonUserItem(wEnumIndex++);
				if( pIServerUserItem==NULL) break;
				if( pIServerUserItem==pTargetUserItem )
				{
					//发送消息
					SendPropertyFailure(pISourceUserItem,TEXT("此道具不可赠送给旁观用户,购买失败！"), 0L,cbRequestArea);

					return false;
				}
			} while (true);
		}
	}

	//道具判断
	switch(wPropertyIndex)
	{
	case PROPERTY_ID_SCORE_CLEAR :			//负分清零
		{
			//变量定义
			SCORE lCurrScore = pTargetUserItem->GetUserScore();
			if( lCurrScore >= 0)
			{
				//变量定义
				TCHAR szMessage[128]=TEXT("");
				if ( pISourceUserItem==pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("您现在的积分已经是非负数，不需要使用负分清零道具！"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]现在的积分已经是非负数，不需要使用负分清零道具！"), pTargetUserItem->GetNickName());

				//发送消息
				SendPropertyFailure(pISourceUserItem,szMessage, 0L,cbRequestArea);

				return false;
			}
			break;
		}
	case PROPERTY_ID_ESCAPE_CLEAR :			 //逃跑清零
		{
			//变量定义
			DWORD dwCurrFleeCount = pTargetUserItem->GetUserInfo()->dwFleeCount;
			if ( dwCurrFleeCount==0 )
			{
				//变量定义
				TCHAR szMessage[128]=TEXT("");		
				if ( pISourceUserItem == pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("您现在的逃跑率已经为0，不需要使用逃跑清零道具！"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]现在的逃跑率已经为0，不需要使用逃跑清零道具！"), pTargetUserItem->GetNickName());

				//发送消息
				SendPropertyFailure(pISourceUserItem,szMessage,0L,cbRequestArea);

				return false;
			}
			break;
		}
	}

	return true;
}

//绑定用户
IServerUserItem * CAttemperEngineSink::GetBindUserItem(WORD wBindIndex)
{
	//获取参数
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//获取用户
	if (pBindParameter!=NULL)
	{
		return pBindParameter->pIServerUserItem;
	}

	//错误断言
	ASSERT(FALSE);

	return NULL;
}

//绑定参数
tagBindParameter * CAttemperEngineSink::GetBindParameter(WORD wBindIndex)
{
	//无效连接
	if (wBindIndex==INVALID_WORD) return NULL;

	//常规连接
	if (wBindIndex<m_pGameServiceOption->wMaxPlayer)
	{
		return m_pNormalParameter+wBindIndex;
	}

	//机器连接
	if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		return m_pAndroidParameter+(wBindIndex-INDEX_ANDROID);
	}

	//错误断言
	ASSERT(FALSE);

	return NULL;
}

//道具类型
WORD CAttemperEngineSink::GetPropertyType(WORD wPropertyIndex)
{
	switch(wPropertyIndex)
	{
	case PROPERTY_ID_CAR:	case PROPERTY_ID_EGG: 	case PROPERTY_ID_CLAP: 	case PROPERTY_ID_KISS: 	case PROPERTY_ID_BEER:
	case PROPERTY_ID_CAKE: 	case PROPERTY_ID_RING:  case PROPERTY_ID_BEAT: 	case PROPERTY_ID_BOMB:  case PROPERTY_ID_SMOKE:
	case PROPERTY_ID_VILLA: case PROPERTY_ID_BRICK: case PROPERTY_ID_FLOWER: 
		{
			return PT_TYPE_PRESENT;
		};
    case PROPERTY_ID_TWO_CARD: 	case PROPERTY_ID_FOUR_CARD:  case PROPERTY_ID_SCORE_CLEAR:     case PROPERTY_ID_ESCAPE_CLEAR:
	case PROPERTY_ID_TRUMPET:	case PROPERTY_ID_TYPHON:     case PROPERTY_ID_GUARDKICK_CARD:  case PROPERTY_ID_POSSESS:
	case PROPERTY_ID_BLUERING_CARD: case PROPERTY_ID_YELLOWRING_CARD: case PROPERTY_ID_WHITERING_CARD: case PROPERTY_ID_REDRING_CARD:
	case PROPERTY_ID_VIPROOM_CARD: 
		{
			return PT_TYPE_PROPERTY;
		};
	}

	ASSERT(false);

	return PT_TYPE_ERROR;
}

//配置机器
bool CAttemperEngineSink::InitAndroidUser()
{
	//机器参数
	tagAndroidUserParameter AndroidUserParameter;
	ZeroMemory(&AndroidUserParameter,sizeof(AndroidUserParameter));

	//配置参数
	AndroidUserParameter.pGameParameter=m_pGameParameter;
	AndroidUserParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	AndroidUserParameter.pGameServiceOption=m_pGameServiceOption;

	//服务组件
	AndroidUserParameter.pITimerEngine=m_pITimerEngine;
	AndroidUserParameter.pIServerUserManager=&m_ServerUserManager;
	AndroidUserParameter.pIGameServiceManager=m_pIGameServiceManager;
	AndroidUserParameter.pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent);

	//设置对象
	if (m_AndroidUserManager.InitAndroidUser(AndroidUserParameter)==false)
	{
		return false;
	}

	return true;
}

//配置桌子
bool CAttemperEngineSink::InitTableFrameArray()
{
	//桌子参数
	tagTableFrameParameter TableFrameParameter;
	ZeroMemory(&TableFrameParameter,sizeof(TableFrameParameter));

	//内核组件
	TableFrameParameter.pITimerEngine=m_pITimerEngine;
	TableFrameParameter.pIKernelDataBaseEngine=m_pIKernelDataBaseEngine;
	TableFrameParameter.pIRecordDataBaseEngine=m_pIRecordDataBaseEngine;

	//服务组件
	TableFrameParameter.pIMainServiceFrame=this;
	TableFrameParameter.pIAndroidUserManager=&m_AndroidUserManager;
	TableFrameParameter.pIGameServiceManager=m_pIGameServiceManager;

	//配置参数
	TableFrameParameter.pGameParameter=m_pGameParameter;
	TableFrameParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	TableFrameParameter.pGameServiceOption=m_pGameServiceOption;

	if(m_pIGameMatchServiceManager!=NULL)
		TableFrameParameter.pIGameMatchServiceManager=QUERY_OBJECT_PTR_INTERFACE(m_pIGameMatchServiceManager,IUnknownEx);

	//桌子容器
	m_TableFrameArray.SetSize(m_pGameServiceOption->wTableCount);
	ZeroMemory(m_TableFrameArray.GetData(),m_pGameServiceOption->wTableCount*sizeof(CTableFrame *));

	//创建桌子
	for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		//创建对象
		m_TableFrameArray[i]=new CTableFrame;

		//配置桌子
		if (m_TableFrameArray[i]->InitializationFrame(i,TableFrameParameter)==false)
		{
			return false;
		}

		if(m_pIGameMatchServiceManager!=NULL)
			m_pIGameMatchServiceManager->InitTableFrame(QUERY_OBJECT_PTR_INTERFACE((m_TableFrameArray[i]),ITableFrame),i);
	}

	if(m_pIGameMatchServiceManager!=NULL)
	{
		if (m_pIGameMatchServiceManager->InitMatchInterface(QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent),m_pIKernelDataBaseEngine,
				(IServerUserManager*)QUERY_OBJECT_INTERFACE(m_ServerUserManager,IServerUserManager),this,this,m_pITimerEngine,&m_AndroidUserManager)==false)
		{
			ASSERT(FALSE);
			return false;
		}
// 		if (m_pIGameMatchServiceManager->InitServerUserManager()==false)
// 		{
// 			ASSERT(FALSE);
// 			return false;
// 		}
// 
// 		if(m_pIGameMatchServiceManager->InitMainServiceFrame(QUERY_ME_INTERFACE(IMainServiceFrame))==false)
// 		{
// 			ASSERT(FALSE);
// 			return false;
// 		}

	}

	return true;
}

//发送请求
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//发送数据
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//插入分配
bool CAttemperEngineSink::InsertDistribute(IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//状态判断
	ASSERT(pIServerUserItem->GetUserStatus()<US_PLAYING);
	if (pIServerUserItem->GetUserStatus()>=US_PLAYING) return false;

	bool bReturn=false;
	if(m_pIGameMatchServiceManager!=NULL) bReturn=m_pIGameMatchServiceManager->OnUserJoinGame(pIServerUserItem,0);
	return bReturn;
}

//敏感词过滤
void CAttemperEngineSink::SensitiveWordFilter(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen)
{
	m_WordsFilter.Filtrate(pMsg,pszFiltered,nMaxLen);
}

//插入用户
bool CAttemperEngineSink::InsertWaitDistribute(IServerUserItem * pIServerUserItem)
{
	//查找
	bool bAdd=true;
	POSITION pos=m_WaitDistributeList.GetHeadPosition();
	while(pos != NULL)
	{
		IServerUserItem *pUserItem = m_WaitDistributeList.GetNext(pos);
		if(pUserItem->GetUserID()==pIServerUserItem->GetUserID())
		{
			bAdd=false;
			break;
		}
	}

	//加入
	if(bAdd) m_WaitDistributeList.AddTail(pIServerUserItem);

	//通知
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_WAIT_DISTRIBUTE,NULL,0);

	return bAdd;
}

//删除用户
bool CAttemperEngineSink::DeleteWaitDistribute(IServerUserItem * pIServerUserItem)
{
	//查找
	POSITION pos=m_WaitDistributeList.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION tempPos = pos;
		IServerUserItem *pUserItem = m_WaitDistributeList.GetNext(pos);
		if(pUserItem->GetUserID()==pIServerUserItem->GetUserID())
		{
			m_WaitDistributeList.RemoveAt(tempPos);
			break;
		}
	}

	return true;
}

//分配用户
bool CAttemperEngineSink::DistributeUserGame()
{
	//用户数判断
	WORD wNeedUserCount=__max(m_pGameServiceAttrib->wChairCount, m_pGameServiceOption->wMinDistributeUser);
	if(m_pGameServiceAttrib->cbDynamicJoin==TRUE)wNeedUserCount=2;
	if(m_WaitDistributeList.GetCount()<wNeedUserCount)return true;

	//随机分配
	for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if ((pTableFrame->IsGameStarted()==true)) continue;

		//桌子状况
		tagTableUserInfo TableUserInfo;
		WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

		//分配判断
		if (wUserSitCount!=0) continue;

		//坐下判断
		if (m_WaitDistributeList.GetCount()>=TableUserInfo.wMinUserCount)
		{
			//变量定义
			WORD wHandleCount=0;
			WORD wWantUserCount=TableUserInfo.wMinUserCount;
			WORD wFreeUserCount=(WORD)m_WaitDistributeList.GetCount();

			//数据调整
			if (m_pGameServiceAttrib->wChairCount>TableUserInfo.wMinUserCount)
			{
				WORD wChairCount=m_pGameServiceAttrib->wChairCount;
				WORD wOffUserCount=__min(wChairCount,wFreeUserCount)-TableUserInfo.wMinUserCount;
				wWantUserCount+=(wOffUserCount > 0) ? (rand()%wOffUserCount) : 0;
			}

			//坐下处理
			POSITION pos=m_WaitDistributeList.GetHeadPosition();
			while(pos!=NULL)
			{
				//变量定义
				WORD wChairID=pTableFrame->GetRandNullChairID();

				//无效过滤
				if (wChairID==INVALID_CHAIR) continue;

				//坐下处理
				POSITION tempPos=pos;
				IServerUserItem *pUserItem = m_WaitDistributeList.GetNext(pos);
				if(pUserItem && pUserItem->GetUserStatus()==US_FREE && (pTableFrame->PerformSitDownAction(wChairID,pUserItem)==true))
				{
					//删除排队
					m_WaitDistributeList.RemoveAt(tempPos);

					//设置变量
					wHandleCount++;

					//完成判断
					if (wHandleCount>=wWantUserCount) 
					{
						return true;
					}
				}
			}

			if(wHandleCount > 0) return true;
		}
	}

	return true;
}

//设置参数
void CAttemperEngineSink::SetMobileUserParameter(IServerUserItem * pIServerUserItem,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	if(wPageTableCount > m_pGameServiceOption->wTableCount)wPageTableCount=m_pGameServiceOption->wTableCount;
	pIServerUserItem->SetMobileUserDeskCount(wPageTableCount);
	pIServerUserItem->SetMobileUserRule(wBehaviorFlags);
}

//群发数据
bool CAttemperEngineSink::SendDataBatchToMobileUser(WORD wCmdTable, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//枚举用户
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//过滤用户
		IServerUserItem *pIServerUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIServerUserItem==NULL) continue;
		if(!pIServerUserItem->IsMobileUser()) continue;

		//定义变量
		WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
		WORD wTagerTableID = pIServerUserItem->GetTableID();
		bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
		bool bRecviceGameChat = ((wMobileUserRule&RECVICE_GAME_CHAT)!=0);
		bool bRecviceRoomChat = ((wMobileUserRule&RECVICE_ROOM_CHAT)!=0);
		bool bRecviceRoomWhisper = ((wMobileUserRule&RECVICE_ROOM_WHISPER)!=0);

		//状态过滤
		if(pIServerUserItem->GetUserStatus() >= US_SIT)
		{
			if(wTagerTableID != wCmdTable)continue;
		}

		//聊天过滤
		if(wSubCmdID==SUB_GR_USER_CHAT || wSubCmdID==SUB_GR_USER_EXPRESSION)
		{
			if(!bRecviceGameChat || !bRecviceRoomChat) continue;
		}
		if(wSubCmdID==SUB_GR_WISPER_CHAT || wSubCmdID==SUB_GR_WISPER_EXPRESSION)
		{
			if(!bRecviceRoomWhisper) continue;
		}

		//部分可视
		if(!bViewModeAll)
		{
			//消息过滤
			if(wSubCmdID==SUB_GR_USER_ENTER && wCmdTable==INVALID_TABLE) continue;
			if(wSubCmdID==SUB_GR_USER_SCORE && pIServerUserItem->GetUserStatus() < US_SIT) continue;
			
			//定义变量
			WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
			WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();

			//状态消息过滤
			if(wCmdTable < wTagerDeskPos) continue;
			if(wCmdTable > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//发送消息
		SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//群发用户信息
bool CAttemperEngineSink::SendUserInfoPacketBatchToMobileUser(IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));

	//用户属性
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//用户属性
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

	//用户状态
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//用户局数
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;

	//用户成绩
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
	pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

	//叠加信息
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

	//发送数据
	WORD wHeadSize=sizeof(tagMobileUserInfoHead);
	SendDataBatchToMobileUser(pUserInfoHead->wTableID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());

	return true;
}

//发可视用户信息
bool CAttemperEngineSink::SendViewTableUserInfoPacketToMobileUser(IServerUserItem * pIServerUserItem,DWORD dwUserIDReq)
{
	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
	WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
	WORD wTagerTableID = pIServerUserItem->GetTableID();
	WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
	WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();
	bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
	if(wTagerDeskCount==0) wTagerDeskCount=1;

	//枚举用户
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//过滤用户
		IServerUserItem *pIUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIUserItem==NULL || (dwUserIDReq==INVALID_CHAIR && pIUserItem==pIServerUserItem)) continue;
		if(dwUserIDReq != INVALID_CHAIR && pIUserItem->GetUserID() != dwUserIDReq) continue;

		//部分可视
		if(dwUserIDReq==INVALID_CHAIR && !bViewModeAll)
		{
			if(pIUserItem->GetTableID() < wTagerDeskPos) continue;
			if(pIUserItem->GetTableID() > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//定义变量
		tagUserInfo * pUserInfo=pIUserItem->GetUserInfo();
		ZeroMemory(cbBuffer,sizeof(cbBuffer));
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));

		//用户属性
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//用户属性
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//用户状态
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//用户局数
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//用户成绩
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIUserItem->GetFrozenedScore();

		//叠加信息
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//发送数据
		WORD wHeadSize=sizeof(tagMobileUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//手机立即登录
bool CAttemperEngineSink::MobileUserImmediately(IServerUserItem * pIServerUserItem)
{
	//查找桌子
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//无效过滤
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;

		//用户坐下
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//失败
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("没找到可进入的游戏桌！"),REQUEST_FAILURE_NORMAL);
	return true;
}

//发送系统消息
bool CAttemperEngineSink::SendSystemMessage(CMD_GR_SendMessage * pSendMessage, WORD wDataSize)
{
	//消息处理
	ASSERT(pSendMessage!=NULL);

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength);
	if (wDataSize!=sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength) 
		return false;

	//所有房间
	if(pSendMessage->cbAllRoom == TRUE)
	{
		pSendMessage->cbAllRoom=FALSE;
		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_SYSTEM_MESSAGE,pSendMessage,wDataSize);
	}
	else
	{
		//发送系统消息
		if(pSendMessage->cbGame == TRUE)
			SendGameMessage(pSendMessage->szSystemMessage,SMT_CHAT);
		if(pSendMessage->cbRoom != FALSE)
		{
			WORD wType = (pSendMessage->cbRoom & SMT_CATCH_FISH) ? (SMT_CHAT | SMT_CATCH_FISH) : (SMT_CHAT);
			SendRoomMessage(pSendMessage->szSystemMessage,wType);
		}
	}

	return true;
}

//清除消息数据
void CAttemperEngineSink::ClearSystemMessageData()
{
	while(m_SystemMessageList.GetCount() > 0)
	{
		tagSystemMessage * pRqHead = m_SystemMessageList.RemoveHead();
		if(pRqHead)
		{
			delete [] ((BYTE*)pRqHead);
		}
	}
}

const tagStockInfo* CAttemperEngineSink::GetStockInfo(WORD wTableID, int nType)
{
	if (eStockType_Player == nType)
	{
		return &m_TotalStock;
	}

	if (eStockType_Robot == nType)
	{
		return &m_RobotStock;
	}

	return NULL;
}

void CAttemperEngineSink::ChangeStock(int nType, WORD wTableID, DWORD nUserID, int nStock, int nRevenue, int nCaiJin)
{
	if (eStockType_Player == nType)
	{
		if (m_TotalStock.mStockScore + nStock <= 2100000000L)
			m_TotalStock.mStockScore += nStock;
		if (m_TotalStock.mRevenue + nRevenue <= 2100000000L)
			m_TotalStock.mRevenue += nRevenue;
		if (m_TotalStock.mCaiJin + nCaiJin <= 2100000000L)
			m_TotalStock.mCaiJin += nCaiJin;

		if (nCaiJin < 0)
		{
			m_TotalStock.mCaiJinSent -= nCaiJin;
		}

	}

	if (eStockType_Robot == nType)
	{
		if (m_RobotStock.mStockScore + nStock <= 2100000000L)
			m_RobotStock.mStockScore += nStock;
		if (m_RobotStock.mRevenue + nRevenue <= 2100000000L)
			m_RobotStock.mRevenue += nRevenue;
		if (m_RobotStock.mCaiJin + nCaiJin <= 2100000000L)
			m_RobotStock.mCaiJin += nCaiJin;

		if (nCaiJin < 0)
		{
			m_RobotStock.mCaiJinSent -= nCaiJin;
		}
	}
}

float CAttemperEngineSink::GetUserFactor(unsigned int nUserID)
{
	float fRet=-1.0;
	for (std::list<tagUserLucky *>::iterator iter=m_UserLucky.begin(); iter!=m_UserLucky.end(); iter++)
	{
		tagUserLucky * pElement=*iter;
		if (pElement->dwUserID==nUserID)
		{
			fRet = pElement->fLucky;
			break;
		}
	}

	return fRet;
}

//幸运值
bool CAttemperEngineSink::OnDBGetUserLucky(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(DBR_GR_UserLucky));
	if (wDataSize!=sizeof(DBR_GR_UserLucky)) return false;

	DBR_GR_UserLucky * pUserLucky = (DBR_GR_UserLucky *)pData;

	if (pUserLucky)
	{
		if (dwContextID==-1)
		{
			for (std::list<tagUserLucky *>::iterator iter=m_UserLucky.begin(); iter!=m_UserLucky.end();)
			{
				std::list<tagUserLucky *>::iterator iter2=iter;
				iter++;

				tagUserLucky * pElement=*iter2;
				if (pElement->dwSerial!=pUserLucky->dwSerial)
				{
					delete *iter2;
					m_UserLucky.erase(iter2);
				}
			}
			return true;
		}

		std::list<tagUserLucky *>::iterator iter=m_UserLucky.begin();
		for (; iter!=m_UserLucky.end(); iter++)
		{
			tagUserLucky * pElement=*iter;
			if (pElement->dwUserID==pUserLucky->dwUserID)
			{
				pElement->dwSerial=pUserLucky->dwSerial;
				pElement->fLucky=pUserLucky->fLucky;
				break;
			}
		}

		if (iter==m_UserLucky.end())
		{
			tagUserLucky * pElement = new tagUserLucky;
			pElement->dwUserID=pUserLucky->dwUserID;
			pElement->dwSerial=pUserLucky->dwSerial;
			pElement->fLucky=pUserLucky->fLucky;
			m_UserLucky.push_back(pElement);
		}
	}

	return true;
}

bool CAttemperEngineSink::OnUserMatchSignUPRes(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(DBO_GR_Match_UserSignUp));
	if (wDataSize!=sizeof(DBO_GR_Match_UserSignUp)) return false;
	
	DBO_GR_Match_UserSignUp * pMatchUserSignUp = (DBO_GR_Match_UserSignUp *) pData;	
	if (pMatchUserSignUp->bIsMatchSignUPSuccess == true)	
	{
		CMD_CS_C_MatchSignUp MatchSignUp = {0};
		MatchSignUp.dwUserID = pMatchUserSignUp->dwUserID;
		MatchSignUp.wServerID = m_pGameServiceOption->wServerID;

		//m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_MATCH_SIGNUP, &MatchSignUp, sizeof(CMD_CS_C_MatchSignUp));
	}

	return true;	
}

//存储用户物品
int CAttemperEngineSink::SaveUserItem(IServerUserItem * pIServerUserItem, int nItemIndex)
{
	if (NULL == pIServerUserItem)
	{
		return 0;
	}

	int nItemCount = pIServerUserItem->GetUserItemCount();
	tagUserItem * pUserItem = pIServerUserItem->GetUserItems();

	if (NULL == pUserItem || 0 == nItemCount)
	{
		return 0;
	}

	int nSaveCount = 0;

	for (int i = 0; i < nItemCount; i++)
	{
		if (-1 != nItemIndex && nItemIndex != i)
		{
			continue;
		}

		tagUserItem* pTmpUserItem = pUserItem + i;
		if (NULL == pTmpUserItem)
		{
			break;
		}

		if (pTmpUserItem->lCount == pTmpUserItem->lDBCount)
		{
			continue;
		}
		m_pIDBCorrespondManager->PostDataBaseRequest(0L, DBR_GR_SAVEUSERITEM, 0, pTmpUserItem, sizeof(tagUserItem));

		pTmpUserItem->lDBCount = pTmpUserItem->lCount;
	}

	return nSaveCount;
}

//游戏记录Json格式
void CAttemperEngineSink::SaveGameJson(IServerUserItem * pIServerUserItem, const TCHAR * str, SCORE lScore)
{
	if (NULL==pIServerUserItem || NULL==str) return;
	DBR_GR_GameJson GameJson={0};
	GameJson.dwUserID=pIServerUserItem->GetUserID();
	GameJson.dwKindID=m_pGameServiceOption->wKindID;
	GameJson.dwServerID=m_pGameServiceOption->wServerID;
	GameJson.lPresentGold=lScore;
	_tcsncpy(GameJson.szJson,str,CountArray(GameJson.szJson));
	m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_GAME_JSON,0,&GameJson,sizeof(DBR_GR_GameJson));
}

//在线人数
void CAttemperEngineSink::ShowRealOnlineCount()
{
	CString sOnline=TEXT("");
	sOnline.Format(TEXT("当前在线人数：%d，其中真实玩家：%d，PID=%d"),m_ServerUserManager.GetUserItemCount(),m_ServerUserManager.GetRealUserItemCount(),_getpid());
	CTraceService::TraceString(sOnline,TraceLevel_Info);

	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame == NULL) continue;
		pTableFrame->getRecordCount();
	}
}

//写分并剔除
void CAttemperEngineSink::SaveAndKick()
{
	CMD_GR_KickAllUser KickAllUser={0};

	_sntprintf(KickAllUser.szKickMessage,CountArray(KickAllUser.szKickMessage),TEXT("本房间临时维护，我们将会为您保护好分数，对您造成的不便尽请谅解，请稍后进入本房间或其它房间进行游戏。"));

	//解散所有游戏
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (!pTableFrame->IsGameStarted()) continue;
		pTableFrame->DismissGame();
	}

	tagBindParameter * pBindParameter=m_pNormalParameter;
	for (INT i=0; i<m_pGameServiceOption->wMaxPlayer; i++)
	{
		//目录用户
		IServerUserItem * pITargerUserItem=pBindParameter->pIServerUserItem;
		if (pITargerUserItem==NULL) 
		{
			pBindParameter++;
			continue;
		}
		//发送消息
		SendRoomMessage(pITargerUserItem,KickAllUser.szKickMessage,SMT_CHAT|SMT_EJECT|SMT_CLOSE_LINK|SMT_CLOSE_ROOM);
		pBindParameter++;
	} 
}

tagLotteryInfo * CAttemperEngineSink::GetLotteryInfo()
{
	return &m_LotteryInfo;
}

tagMobileMoney * CAttemperEngineSink::GetMobileMoney()
{
	return &m_MobileMoney;
}

VOID CAttemperEngineSink::AndriodSitTableAssign()
{
	for (UINT i=0; i<3;)
	{
		m_PrecedenceTableArray[i]=rand()%10;
		UINT j=0;
		for(j=0; j<i; j++)
		{
			if(m_PrecedenceTableArray[i] == m_PrecedenceTableArray[j])
			{
				break;
			}
		}

		if(j == i)
		{
			i++;
		}
	}
}

//赢得龙珠碎片
bool CAttemperEngineSink::WinDragonPiece(IServerUserItem * pIServerUserItem, SCORE lWinDragonPiece, SCORE lWinScore)
{
	if (pIServerUserItem == NULL || lWinDragonPiece < 0 || lWinScore < 0)
	{
		printf("%s pIServerUserItem=%p lWinDragonPiece=%lld lWinScore=%lld\n", __FUNCTION__, pIServerUserItem, lWinDragonPiece, lWinScore);
		return false;
	}

	DBR_GR_UserWinDragonPiece UserTakeScore = {0};

	UserTakeScore.dwUserID=pIServerUserItem->GetUserID();
	UserTakeScore.lWinDragonPiece=lWinDragonPiece;
	UserTakeScore.lWinScore=lWinScore;
	UserTakeScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserTakeScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTakeScore.szMachineID));

	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_WIN_DRAGON_PIECE,pBindParameter->dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

	return true;
}

//赢得珍珠
bool CAttemperEngineSink::WinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount)
{
	if (pIServerUserItem == NULL || nPearlType < 0 || nPearlCount < 0)
	{
		printf("%s pIServerUserItem=%p nPearlType=%d nPearlCount=%d\n", __FUNCTION__, pIServerUserItem, nPearlType, nPearlCount);
		return false;
	}

	DBR_GR_WinPearl WinPearl1 = {0};
	WinPearl1.dwUserID = pIServerUserItem->GetUserID();
	WinPearl1.nPearlType = nPearlType;
	WinPearl1.nPearlCount = nPearlCount;

	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());

	//投递请求
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_WIN_PEARL,pBindParameter->dwSocketID,&WinPearl1,sizeof(WinPearl1));

	return true;
}

//技能处理
bool CAttemperEngineSink::OnTCPNetworkMainSkill(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	switch (wSubCmdID)
	{
	case SUB_GRR_QUERY_SKILL:
		{
			if (wDataSize != 0)
			{
				return false;
			}

			DBR_GR_QuerySkill QuerySkill = {0};
			QuerySkill.dwUserID = pIServerUserItem->GetUserID();

			m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_QUERY_SKILL,dwSocketID,&QuerySkill,sizeof(QuerySkill));

			return true;
		}
	case SUB_GRR_USE_SKILL:
		{
			if (wDataSize != sizeof(CMD_GRR_UseSkill))
			{
				return false;
			}

			CMD_GRR_UseSkill * pUseSkill = (CMD_GRR_UseSkill *)pData;

			DBR_GR_UseSkill UseSkill = {0};
			UseSkill.dwUserID = pIServerUserItem->GetUserID();
			UseSkill.cbSkillID = pUseSkill->cbSkillID;

			m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USE_SKILL,dwSocketID,&UseSkill,sizeof(UseSkill));

			return true;
		}
	}

	return true;
}

//系统赠送技能
bool CAttemperEngineSink::SkillGive(IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem == NULL)
	{
		return false;
	}

	DBR_GR_GiveSkill GiveSkill = {0};
	GiveSkill.dwUserID=pIServerUserItem->GetUserID();
	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_SKILL_GIVE,pBindParameter->dwSocketID,&GiveSkill,sizeof(GiveSkill));

	return true;
}

//新手引导奖励
bool CAttemperEngineSink::OnNewGuidGift(IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem == NULL)
	{
		return true;
	}

	DBR_GR_NewGuidGift NewGuidGift = {0};
	NewGuidGift.dwUserID=pIServerUserItem->GetUserID();
	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_NEW_GUID_GIFT,pBindParameter->dwSocketID,&NewGuidGift,sizeof(NewGuidGift));

	return true;
}

//发送比赛积分
bool CAttemperEngineSink::SendMatchScore(IServerUserItem * pIServerUserItem, SCORE lMatchScore)
{
	if (pIServerUserItem == NULL)
	{
		return true;
	}

	CMD_CS_C_MatchScore MatchScore = {0};
	MatchScore.dwUserID = pIServerUserItem->GetUserID();
	MatchScore.lMatchScore = lMatchScore;
	m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_MatchScore, &MatchScore, sizeof(MatchScore));

	return true;
}

//发送全服广播
void CAttemperEngineSink::SendPlatformMessage(LPCTSTR lpszMessage, BYTE cbLevel)
{
	CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
	VIPUpgrade.cbType = cbLevel;

	strncpy(VIPUpgrade.szTrumpetContent, lpszMessage, _countof(VIPUpgrade.szTrumpetContent));

	if (m_pITCPSocketService)
	{
		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
	}
}

bool CAttemperEngineSink::OnTCPNetworkGetBigReward(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	switch (wSubCmdID)
	{
	case SUB_GRR_GET_BIG_REWARD:
		{
			CMD_GR_CLOWNBIGREWARD stBigReward;
			ZeroMemory(&stBigReward, sizeof(stBigReward));
			stBigReward.lBigReward = m_lBigReward;
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_MANAGE,SUB_GR_CLOWN_BIG_REWARD,(VOID *)&stBigReward,sizeof(CMD_GR_CLOWNBIGREWARD));

			return true;
		}
	}

	return true;
}

//获取用户道具
void CAttemperEngineSink::GetUserItemCount(IServerUserItem * pIServerUserItem, int nType)
{
printf("%s %d\n", __FUNCTION__, __LINE__);
	DBR_GetUserItemCount GetUserItemCount1 = {};
	GetUserItemCount1.dwUserID = pIServerUserItem->GetUserID();
	GetUserItemCount1.nType = nType;

	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GET_USER_ITEM_COUNT,pBindParameter->dwSocketID,&GetUserItemCount1,sizeof(GetUserItemCount1));
}


//保存用户道具
void CAttemperEngineSink::SaveUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount)
{
printf("%s %d\n", __FUNCTION__, __LINE__);
	DBR_SaveUserItemCount SaveUserItemCount1 = {};
	SaveUserItemCount1.dwUserID = pIServerUserItem->GetUserID();
	SaveUserItemCount1.nType = nType;
	SaveUserItemCount1.lCount = lCount;

	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_SAVE_USER_ITEM_COUNT,pBindParameter->dwSocketID,&SaveUserItemCount1,sizeof(SaveUserItemCount1));
}

bool CAttemperEngineSink::OnWinBigReward(IServerUserItem * pIServerUserItem,int nScore)
{
	if (pIServerUserItem == NULL)
	{
		return false;
	}

	CMD_GR_CLOWNFAKEANNOUNCE stFakeAnnounce;
	ZeroMemory(&stFakeAnnounce, sizeof(CMD_GR_CLOWNFAKEANNOUNCE));
	_sntprintf(stFakeAnnounce.szClownFakeAnnounceMent,CountArray(stFakeAnnounce.szClownFakeAnnounceMent),
		TEXT("恭喜【Vip%d%s】在超级海盗中赢得%ld万大奖。"),pIServerUserItem->GetVipLevel(),pIServerUserItem->GetNickName(),nScore);
	//CTraceService::TraceString(stFakeAnnounce.szClownFakeAnnounceMent,TraceLevel_Normal);

	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_MANAGE,SUB_GR_CLOWN_FAKEANNOUNCEMENT,(VOID *)&stFakeAnnounce,sizeof(CMD_GR_CLOWNFAKEANNOUNCE),BG_COMPUTER);	
	CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
	if (pIServerUserItem->GetVipLevel()>0)
	{
		_sntprintf(VIPUpgrade.szTrumpetContent,CountArray(VIPUpgrade.szTrumpetContent),TEXT("#ffffff恭喜【#ff0000Vip%d%s#ffffff】在超级海盗中赢得#ff0000%d万#ffffff大奖，#L01点击此处赢大奖！"),pIServerUserItem->GetVipLevel(),pIServerUserItem->GetNickName(),nScore);
	}
	else
	{
		_sntprintf(VIPUpgrade.szTrumpetContent,CountArray(VIPUpgrade.szTrumpetContent),TEXT("#ffffff恭喜【#ff0000%s#ffffff】在超级海盗中赢得#ff0000%d万#ffffff大奖，#L01点击此处赢大奖！"),pIServerUserItem->GetNickName(),nScore);
	}
	

	VIPUpgrade.cbType = 35;

	if (m_pITCPSocketService)
	{
		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
	}
	return true;
}

void CAttemperEngineSink::SetTreasureMapCount(IServerUserItem * pIServerUserItem,int nCount)
{
	if (pIServerUserItem == NULL)
	{
		return;
	}
	int nTreasureMapCount = pIServerUserItem->GetTreasureMapCount();
	nTreasureMapCount = nTreasureMapCount+nCount;
	if (nTreasureMapCount<0)
	{
		nCount = 0;
		nTreasureMapCount = 0;
	}
	pIServerUserItem->SetTreasureMapCount(nTreasureMapCount);
	DBR_GR_Update_Treasuremap RequestInfo = {0};
	RequestInfo.dwUserID = pIServerUserItem->GetUserID();
	RequestInfo.nCount = nCount;
	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_UPDATE_TREASUREMAP,pBindParameter->dwSocketID,&RequestInfo,sizeof(RequestInfo));

	return;
}

bool CAttemperEngineSink::OnTCPNetworkSubFirstEnterSuper(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem == NULL)
	{
		return false;
	}
	DBR_GR_First_Enter_Super sRequestInfo={0};
	sRequestInfo.dwUserID = pIServerUserItem->GetUserID();
	m_pIDBCorrespondManager->PostDataBaseRequest(sRequestInfo.dwUserID,DBR_GR_FIRST_ENTER_SUPER,dwSocketID,&sRequestInfo,sizeof(sRequestInfo));
	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubMatchSignUP(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GRR_MatchSignUP));
	if (wDataSize!=sizeof(CMD_GRR_MatchSignUP)) return false;

	CMD_GRR_MatchSignUP * pUserMatchSignUP = (CMD_GRR_MatchSignUP *)pData;
	//CMD_GRO_MatchSignupRes stUserMatchSignUpRes = {0};
	//if (m_bMatchSignUpMark == false)
	//{
	//	//比赛时间尚未开始 发送提示信息
	//	stUserMatchSignUpRes.dwUserID = pUserMatchSignUP->dwUserID;
	//	stUserMatchSignUpRes.bIsSignUpSuc = false;
	//	lstrcpyn(stUserMatchSignUpRes.szDescribeString,TEXT("报名时间尚未开始"),CountArray(stUserMatchSignUpRes.szDescribeString));

	//	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_USER,SUB_GR_MATCH_SIGNUP_RES,
	//		&stUserMatchSignUpRes,sizeof(CMD_GR_UserMatchSignUP_Res));
	//	return true;
	//}

	CMD_CS_C_MatchSignUp MatchSignUp = {0};
	MatchSignUp.dwUserID = pUserMatchSignUP->dwUserID;
	MatchSignUp.wServerID = m_pGameServiceOption->wServerID;
	MatchSignUp.nMatchID = pUserMatchSignUP->nMatchID;

	m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_MATCH_SIGNUP, &MatchSignUp, sizeof(CMD_CS_C_MatchSignUp));

	//m_pIDBCorrespondManager->PostDataBaseRequest(stUserMatchSignUP.dwUserID,DBR_GR_MATCH_USERSIGNUP,dwSocketID,
	//	&stUserMatchSignUP,sizeof(stUserMatchSignUP));

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubMatchGetPrize(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GPR_MatchGetMatchPrise));
	if (wDataSize!=sizeof(CMD_GPR_MatchGetMatchPrise)) return false;

	CMD_GPR_MatchGetMatchPrise * pGetMatchPrise = (CMD_GPR_MatchGetMatchPrise *)pData;
	DBR_GR_Match_Get_Prize stMatchGetPrize={0};
	stMatchGetPrize.dwUserID = pGetMatchPrise->dwUserID;
	stMatchGetPrize.nMatchID = pGetMatchPrise->nMatchID;
	lstrcpyn(stMatchGetPrize.szMatchData,pGetMatchPrise->szMatchData,sizeof(stMatchGetPrize.szMatchData));
	printf("玩家id=%d,比赛场次=%d,比赛日期=%s\n",stMatchGetPrize.dwUserID,stMatchGetPrize.nMatchID,stMatchGetPrize.szMatchData);
	m_pIDBCorrespondManager->PostDataBaseRequest(pGetMatchPrise->dwUserID,DBR_GR_MATCH_GET_PRIZE,dwSocketID,&stMatchGetPrize,sizeof(DBR_GR_Match_Get_Prize));

	return true;
}

void CAttemperEngineSink::UpdateUserKValueAndIntegration(IServerUserItem * pIServerUserItem,int nKvalue,int nIntegration)
{
	if (pIServerUserItem == NULL)
	{
		return;
	}
	pIServerUserItem->UpdateUserKValueAndIntegration(nKvalue,2);
	DBR_GR_Update_KValueAndIntegration RequestInfo = {0};
	RequestInfo.dwUserID = pIServerUserItem->GetUserID();
	RequestInfo.nKValue = nKvalue;
	RequestInfo.nIntegration = nIntegration;
	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_UPDATE_KVALUEANDINTEGRATION,pBindParameter->dwSocketID,&RequestInfo,sizeof(RequestInfo));

	return;
}

void CAttemperEngineSink::UpdateUserMaxKValue(IServerUserItem * pIServerUserItem,int nCount)
{
	if (pIServerUserItem == NULL)
	{
		return;
	}
	DBR_GR_Update_MaxKValue RequestInfo = {0};
	RequestInfo.dwUserID = pIServerUserItem->GetUserID();
	RequestInfo.nCount = nCount;
	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_UPDATE_MAXKVALUE,pBindParameter->dwSocketID,&RequestInfo,sizeof(RequestInfo));

	return;
}

void CAttemperEngineSink::UpdateUserIntegrate(IServerUserItem * pIServerUserItem,int nAddScore)
{
	if (pIServerUserItem == NULL)
	{
		return;
	}

	printf("Score update, UserID = %ld, AddScore = %d\n", pIServerUserItem->GetUserID(), nAddScore);

	if (m_nCurrentMatchID == 0)
	{
		printf ("Invalid Current Match ID\n");
		return ;
	}

	printf("the status now is %hhu\n", m_cbMatchStatus);

	if (m_cbMatchStatus == MATCH_STATUS_BEGIN)
	{
		for (int i = 0; i < m_vMatchUserInfo[m_nCurrentMatchID - 1].size(); i++)
		{
			if (pIServerUserItem->GetUserID() == m_vMatchUserInfo[m_nCurrentMatchID - 1][i].dwUserID)	
			{
				m_vMatchUserInfo[m_nCurrentMatchID - 1][i].lUserScore = m_vMatchUserInfo[m_nCurrentMatchID - 1][i].lUserScore + nAddScore;
				printf("Add User Score Success, UserScore is %lld\n", m_vMatchUserInfo[m_nCurrentMatchID - 1][i].lUserScore);
				//m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_SCORE_UPDATE,&stMatchScoreUpdate,sizeof(CMD_CS_C_MatchScoreUpdate));
				break;
			}
		}
	}

	return;
}

void CAttemperEngineSink::GetNextMatchInfo(tagMatchConfigItem &MatchConfigItem)
{
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);
	for (std::list<tagMatchConfigItem>::iterator iter=m_MatchConfigItemList.begin(); iter!=m_MatchConfigItemList.end(); iter++)
	{
		int nMatchStartTimeHour = (iter->nStartTime)/100;
		int nMatchStartTimeMinute = (iter->nStartTime)%100;
		if (SystemTime.wHour==nMatchStartTimeHour && SystemTime.wMinute<=nMatchStartTimeMinute)
		{
			MatchConfigItem.nApplyCost = iter->nApplyCost;
			MatchConfigItem.nEndTime = iter->nEndTime;
			MatchConfigItem.nGameTime = iter->nGameTime;
			MatchConfigItem.nMatchNum = iter->nMatchNum;
			MatchConfigItem.nMatchType = iter->nMatchType;
			MatchConfigItem.nMatchUserCount = iter->nMatchUserCount;
			MatchConfigItem.nStartTime = iter->nStartTime;
			return;
		}
	}
	return;
}

void CAttemperEngineSink::GetUserMatchRewardInfo(tagMatchRewardConfigItem & MatchConfigItem,int Rank,int nMatchType)
{
	for (std::list<tagMatchRewardConfigItem>::iterator iter=m_MatchRewardConfigItemList.begin(); iter!=m_MatchRewardConfigItemList.end(); iter++)
	{
		if ((Rank>=iter->nRankStart&&Rank<=iter->nRankEnd)&&iter->nMachType==nMatchType)
		{
			MatchConfigItem.nMachType = iter->nMachType;
			MatchConfigItem.nRankStart = iter->nRankStart;
			MatchConfigItem.nRankEnd = iter->nRankEnd;
			for (int i=0;i < 10;++i)
			{
				MatchConfigItem.nReward[i] = iter->nReward[i];
			}
			return;
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////
