#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

char *  g_pszFakeName[] = {"速战速决","光辉岁月","因帅被判无妻","闲情逸致","不懂爱的人","笑饮砒霜","跟往事干杯","勇者之王","胸有大痣","久了就旧了","迷失在人海","邪恶老烟枪","第二炮半价","老衲下山采花","天然野生帅哥","一岁就很帅","奔腾草泥马","真心喂过狗","酷派哥","杰叭与不叭都一样","ushdhdebgd","bvddfghhjj","augvss","dghjjhg","142588","Hggtvh","第一副","减肥茶","hjij66663","dfghhbbbn","cvgrdd","vuhghjj","0459zd","无雨","咯咯大笑不止","yvdexm","suggshsh","ydrrdrrr","bxbdveb","okjkohk","jpz011","jpz009","杰爱果子","6647568","duliang","Hsshshsjs","zg747606","Csydhsshshs","估计老婆","ghjeeff","李娜999","369077","nnjxjje","苦命卡纳克纳","轰轰烈烈","水泥七彩虹","sdjp18","彩金小王子","9467852","Ywijhmh","最后十块","hdghxvgdv","dududu","dtghhjjn","Zhengjing6","心灵捕手","预告繁花似锦","shouldhave","铁路局模型","沓沓","ffuvuc","nimala","知足常乐","小马8287336","勇猛无比","ai5215","wqasewa","lw7777lw","Qsadsrff","学渣代言人","来地球逛逛","保安大队长","花落知多少","顾我心安","浅黛微妆","脸红气息","d调的华丽","恭喜发财","不是迩的错","请叫我大叔","活在当下丶BSW","大尾巴狼","92萌叔叔","y1个人也能活","潇洒的微笑","世上没有如果","铭记这段殇","暧昧控","格式化丶孤单","_少年已不年少","淡忘","领悟","留点小胡须","戒浮戒躁","adsferf","男儿当自强","绝世淫鱼","师太借个吻","浅唱呐回忆","半岛酒店","喵了个HelloKitty","跟寂寞和好","飞龙在天","闹钟你别闹","人心太拥挤","慕后煮屎者","机器人的爸爸","追着幸福跑","完美舞步","薰衣草的眼泪","说谎我会笑","海绵宝宝","苏格拉没有底","山炮中的极品","刺痛硪的心","留不住的都滚","冷酷的我忐忑","狠狠平凡","斯人独憔悴","过客匆匆","惜笑红颜罪","曾相守勿忘","社会狠残酷","墨小颜","腻了就滚吧","墨墨","倚楼听雨声","浪里个浪","一岁就很帅","囚心176","不气补脑","暮色微凉","指尖落樱舞^_^","举个栗子","人来狗往","人美B遭罪","粗人一枚","凉水刺喉","梦要自己扛","你别皱眉","孤人眠","猪小哼","蔷薇仙子","开心就好","向地狱狂奔","不期而遇","开心就好","不期而遇","再笑乜是伤","手起刀落人抬走","半明媚半忧伤","[学渣代言人]","资深宅男","爱要怎么喊停","开心就好","风情乱种","拥久则安","惊鸿过影","月下唱离歌","年少猖狂必成皇","孤身撑起一片天","逗比卖萌无所不能","你不配情深","正在输入中","鱼筷到碗里来","不高丶不富丶不帅","我在北京地铁战","深海没有雨季","低调糕富帅","梅西0012","请你次辣条","没刘海照样拽","带我装逼带我飞","隔壁王学长","装逼不适合你","有钱就是任性","洞房不败","嘟嘟糖果","蜡笔小旧","骑白猪的王子","别低贱了自己","河南鱼神灬","想赢别喊停","全国帅比代表","柠檬草味道","海倒过来是天","正太大叔","阳光气质男","愿得一人心","爱情别虚伪","钓鱼佬888","很爷们","小草先森","我爱武则天","凄凉夜太美","花落丶莫相离","逍遥捕鱼","马六甲捕鱼","地狱使者","狂战天下","为自己而活","花心本人戒了","妞给根烟抽呗","菊花少年","我是你的男神","夜凄凉","童话皆是假","天比不过海的蓝","冬约夏至","人鬼殊途","闲来雅叙","纪委叫我混社会","往事如烟","终止放荡","拜拜爱过","到此为止","慢慢习惯自然","一首简单的歌","霸占女厕所","依然在起","逝水无痕","怡红院花魁","低调沉默者","冰动尘心","沸水中的雪","爱情式男人","狠狠爱","金鱼的眼泪","小妖","企鹅先生","鳄鱼喵喵叫","忧伤嘉年华","隗语琴","一朵寂寞的花","思念是一种病","心不动则不痛","90后丨帅男","爱到了心痛","夜美的凄凉","现实太现实","心如此悲伤","迷惘的沿途","注定一个人","情话大师","笑敬过往","茶凉缘尽","蕾丝女Humane","扯淡的微笑","谎言谁都会说","等不到的你","让寂寞远行","单身天涯","_浅蓝色De夏","水清云淡","会很幸福","醉卧美人膝","踏雪寻梅","竺建柏","竹影清风","残荷听雨","天镜云生","烟锁寒楼","春眠不觉晓","蝶梦无边","清水幽萍","漾漾涟漪","漫游者","幽逝无痕醉","上善若水","康鸿博","王菲追我半条街","丛宇文","风袖薇香","故乡的云","柏浩大","素颜倾城几度妖娆","寂寞累了","页落","游吟诗人","夜的不平凡","遗憾的那段情","优柔寡断","爱生活爱拉登","逆光少年","慕容听兰","哥特式祸水","男人也会不坚强","正儿八经小骚男","小男人大智慧","安静的美男子","这么近那么远","岁月乱了浮华","久居孤岛无人问","从不低调","藐视大地","微微一笑醉红尘","有些爱划不来","夏天那抹蓝","小新只属于蜡笔","大爱斯文","情飞扬","空心少年","心在寂寞","哆啦C梦","枯萎罂粟花","友谊不毕业","梦入他心","偷心大盗","蛋卷","再回首已陌路","尾戒的爱","亚里士多缺德","造化弄人","颜小熙","未完待续","皇上是喜脉","永不毕业的友谊","你是我的信仰","对不起查无此人","兜兜里有糖","100纯二货","捕鱼达人就是俺","裸泳的鱼","善解人衣","帅的想跳楼","高冷de逗逼","加载中99","蒙牛没我牛","正在缓冲","Royal大懒猫","小土逗","萌妞优酸乳","咿呀咿呀哟","骑蜗牛撵大象","爱赢才会拼","转让半包辣条","连起wifi聊死你","丑的无可挑剔","笑傲任我行","摸扎算命","坎坎坷坷乐呵呵","姓坚名强","男人站直别趴下","心向阳无谓伤","我为自己代言","破茧成蝶","青春不言败","哥4拽","说爱太烫嘴","辣条协会会长i","暧昧乱人心","泡八喝九说十话","向着幸福钱进","无聊也不聊","法克鱿","梅川酷紫","勿忘心安","孤毒","油炸皮卡丘","巷尾暖阳白衬衫","转身说再见","砸锅卖铁来上网","残缺的温柔","装逼界扛把子","一颗小逗比","冷眸笑红尘","黑名单欢迎你","诗酒趁年少","把酒临风","米阳光","ugugugw","快捷键快快","njf566","蓄势待发","jedndbj","liangshanbo","tugugug","hsjsjhsj","gjefduf","3ed4dx","wc80077","hvcdsf2fhjjj","eudjejm","dounimei","想逆袭一下","gbthghgihhh","sdjp16","ddtyyuff","junjun520","dhxydgennx","打鱼铺子","vyvyvubvb","瑞普汽车","udndjdjfidj","德国等发达国家","柳岩","地狱13","dhfjshs","小刚王者","123456沫沫","hduderh","HRsitser","jxx41115","ytfghrd","nishikou44","sgxxhh","qtjmxj","hdhd222","hcijfnkddd","eedddsjjejsee","qianerdenger","wwhuhddhh","迷津桥头","xfghrdcjkkm","liuyangming8","zo747nii","uddjjd2222","幽幽释然","他墨迹咯哦","青梅骑着竹马","喵^^喵","大懒猫","欢迎骚扰","一颗卤蛋_","喂说你呢","迩闹够了没有","禽小兽^^","黯然销魂掌","上帝是个妞","骑母猪闯红灯","我是二逼怕谁","弹走鱼尾纹","情水深王八多","海誓山盟屁丶","来盘烤翅","屎里逃生","32号我娶你","兔比南菠丸","且行且珍惜","骑母猪闯天下","格子控","爱如困兽","无名指忧伤","浪哩个浪","不一样的烟火","少年先疯队","爱我者昌","戒爱","记忆斑驳了","心岛未晴","我是路人甲","其实我不傻","huxian888","xsssssj","zl1382","好运伴我赢","卢造么123","打不死卸载","iuy9688","abcdbb","wz199611","夜游鱼神","dq2563","ssseezzz","zo8787","jzk130708","tu908553","gyffd3555","xgfkfhg","hnhfguuu7","mnbhjkl123","bnnhgtffff","123djj","huxian777","accdaaaa","严建设","喵喵喵喵喵","wwwewweq","dq13468","123djm","023123vvkfjrt","x814420053","zzzjjj88","Gujnnj","kkjnxkbbji","uvf67i8","迷你彬123","Chu兔吐太冷了","华而不实1","rgijgg","jsnsnsnxs","孩儿们给爷倒茶","甜美即可","rddgg448","捕鱼梦琪","vbhgyunjik","jjdjdde","fghjkkhj","bhdhvsge","hdbxbdjdn","vgy123580","nimeim","大大的炮123","Zhangge","e888888e","kkkeerp","ffgtgtgw","nsnsnsnd","仅此而已yyy","hgfu8765","没事找事i","大战三百年","123z4g5m","r561826574","zhaihao","cccxxxz","fxfgg1478","nsnwwww","wc88imdii","jxhgdgd","xcv11111","ygffvh","jw77178ej","aiaiaiai","tuvf668","bdu6wh8i","你好点","jjjjnjnnh","ftygfhuh","ycyfhchvj","jspeibipo1","hbdbjdhsh","我行我素464","gddyjh","鱼雷炸了","yangyan","fig8f8f8","黄飞38","4ch6bj","eraaatj","gsggshhsh","我就不是你了","jssnsnseee","33441122","srffgvhhj","看看咯磨破","优雅气质","zsyzjl123","老鼠也","ghjcgcg","xbhdhdjzj","rgyihf","飞龙008","dtfhvhh","4r5668","jkajtdnw","rfguiiooll","cyydgj","你咯logo","说今天恢复","心情小跟班","几楼出来聊聊","6869583","12rsftgg","nsnsnsss","火柴0317","2271343231","海滩度假","fdsuhggji","黑天使","技术监督局的","姓柳名下惠","xiaowan","鱼雷8","dingggnajhg","jhffddhhj","bdhchhfyd","小孩纸的J","gucgju","d666666d","呒所","好方法好几年","jyfddt","无爱过","小宝宝萌","gugugyyj","努力了就行了吧","gghhjnv","Dingsidang","有哦咯一样","w24j12x11","gdjjjrbfd","dfvyyhj","引吭高歌更好","jdhgejdvd","teghehdh","787yuyy7t6y7","Xnxidsn","15515515wu","a895558","pjantgan","jsjsjssjshsh","tyu677","jhdhdhfn","12315q","我我我你说11","cm摸咯血po","xvxgdgd","虞姬姐","dmwgua","vy316419js","李阿丽","ttd3467","美女配色","king马克大卫","网撒千条鱼","dhhvdgh","章子怡12323","jdhfhdjhdve","聊咯合谋ni","鱼雷","fyhbhh","dssryhg","lkkijo","给我下大鱼","lovemm","ftgmjg","dgdgueuege","中华六","大家觉得仿佛好","qingang1","ysgeywgjsh","d12332","a8957480","gfh654","vjghcf","jdhdbbdbddbn","wzh530","qwevbjk123","韩国发货","bae111鱼雷","huzijjdajjda","Xiao鱼","dmtptdt","Bxjsne","也许会笑呵呵","hcheue369","自觉培养","xl461126464","gdjhdtuf","yiyuhen","asz898","浙江杭州","w7777lw","w6666lw","xtrxtrysss","ss1314520","jjuuiju","同乐山西","杨芙玲","yyyyyyying","etwyug","只是匿名","来咯管15745","fhfbfcbdnd","548064","老朋友a","包aaaaaa","gggfdee","111111w","wyx333","jxl1314","12377y","功夫DJ","jhcrty","rrttyyuuii","梁子998","vcdtgb","聪少666","罗志","dtugdrj","ghssef","莉香姐","nhfdry","n00544","hbvfdewsxz","hsgech","ghggggb","jsnsmddd","hgtyhg6","第二炮免费","裹脚布","11111f","seezddeewe","dongzi5","芬芳岁月","刚哥无敌版本","a15fgcfff","句话不","hhhyivkv","gghgggh","李即将","yhgghl","qwsddff","dongzi4","252525nn","yyegggdggh","frgji5536","a2863115082","usjejesww","tszxhs","w6719625","zxcvbnmlinb","wrjfiydi","a65001234","外边呢11","祖玛教主110","a15070041","dongzi3","fmfjhhjyfj","老狼来也","专家打鱼","uoa5866999","大鱼快下来","打算鱼","jhjhuuhjj","778899oo","aaz1234","滑冰场","djjjjjkk","hahaasre","赖忠祥","u好v健康","尽管非官方","puj2888856","Hdjfjfjfjf","139968146","agily16","777777f","xx09616","yesyes777","sssszxd","a15070041","hai1234567","ghjhvbn","q6445320","hehe2520m","dongzi1","lililiuliu","我就不相知11","赵云子龙","hhhhh222222","拖拉LadyGaga","FZMCZD","tchvds","才几点继续","59383566kk","mo1363825","lukais62868","啊哈哈哈哈哈","490677","小马如何","孤胆英雄","踩踩踩踩c","dfghhjjv","开车快点快点","juslljz","记得查看宝宝","xxxdfrs","wzh158","tyffggtdsfg","agily15","djdkww","巅峰丶捕鱼","wangyui96","huij6366","a15070041kf","jhdhfmf","jdshh34","yuan137094","很关键核心","huozhi","碰到你的活","ghgffgniu6fg","ngfmjuggr","ghjjgvh","杨桃","奖金里咯了","dghffd","a650se","cdtdff","520336","来咯感觉","666tsfwftstw","今晚打老虎ba","kkriben","cvhhhyhh","hhcdryjj","润肺你猜","下降到8","weqasdewa","ddfhvffg","xdfvvcf","Hejhzhdd","555555f","julijuzu","123456wasn","agily14","huzijjdajj","axuda6500","休息休息xx","jdhhhrhj","888888x","输必鱼扑","江湖","我就去哪了11","ajdjjdjd12","许许","ijuyttti","水泥六月","mdkjdhffj","大好人","610289","fgt123","还有他人的","zk747606","jlh814","诸葛亮jiqiren","天天输","xuziyuan666","我想你啦手机","小高123","横刀向天笑为","opl87072","987丽","liuyang660","243320242","yyysss14","gplwun","jqx123","guigui12","woshishuiq","hzhhbshsh","529077","binbinbin","wccc8207","喝口呼吸一下","hdbfhgfg","shijie789","890890a9","wywy51","mg528885","uhggff","dkeem433","fresrew","水泥五百","fhjjfg","snbxhxjzjjsks","小子们有点数","Ehejhsn","yoyo52000","酒逢知己X","wcc88207","hgffhjf","维生素D","小伟捕鱼","A497396","jpgmwjat","7845124","6tuyffhjj","fghjcfhvcgb","jt12356","性感kk","fhfufhfy","jdbh678","qylpcb","ghjkkkkggjk","hjtpgaujg","woshini","啥时候是个头","xfso222","kk284628","iiii646464","zu74760","m96n649","wc82078","bhjycvg","我落伍图秀秀","qtmkwn","SisterHeRui","xgfhff","xk19890526","xbjdbjdjj","我是不是我","mwtppj","流浪者","y52669","sdjp169","ak47ak","土豪哥哥","你给伤害","ye12356","lilipen","s4gf123","自营每平米","cbgdjc","土豪在此","1987118al","yu达人yu","343591037b","dbbxdee733","和天空","尼亚","宝贝乖不乖","iijoll","cy2234","爆机的节奏i","薛刚超","水泥四月","1q1q2w3e","喂等一下","bb142536","tgtddhr","grhjuhty","23748392","gdfigc","jyujenwt","yangjjj111","a15070041","3wangli","即可好看好","大家的办法","lzc8889","456712aq","wmwmpk","水泥三","jiagang002","东京都7","Hw丶无奈","dd14253","A15132","wanglli2","dcjkfesxvh","QsDsddsf","RMB8000","bhft5678","要ni快点完","wang7409","mwpvajt","Yufyvyyvu","卧铺","buyummm","a1507004174","bffhihy","啊上帝能否","260766","雨蒙蒙杀死","kughjyg","法国恢复","namjdjekdmd","我爱你1314","二十八年","都很方便不","水泥二","hdgdgr","ly510125","a91271","978568","zjsnjzjz","rir9ed","发达的规范","天天向上","556680009","俺的金额对","sdfg168","890223nijuan","qwsjjs","飞龙007","a15070804","eeieokfd","西南地区","a2010222","d等待的那么久","Kl7789jsj","xxdfghj","q866052","z7203313","1wangli","hua花2222","long66635","ghjjjjjjjf","大江大海","345678ii","ghzjs556889","cc806500","jhhgbnjjj","豆腐花","当年的3","yffujg","ly520061","qa866052098","ygfgj999","aa0708043","awangli","水泥一","Fe0909","ppplllaa","小黑脸","dmtpwjt","妹妹醉了","ujjhgcvh","sfvezfd","哦婆婆哦楼哦","jfkfkfmff","poiioooo","第一份互粉互","ptmwmahx","在大多数的","ydhh123","shgdyew"};
//定时器
#define IDI_LOTTERY_INFO 10
#define IDI_PEARL 12
#define IDI_TIMER_CHECK_INTERVAL	13			//比赛时间检测
#define IDI_READ_MATCH_CONFIG		14			//读取比赛配置
#define IDI_MATCH_BEGIN_REMINDER		15			//比赛开始倒计时
#define IDI_MATCH_BEGIN		16			//比赛开始
#define IDI_MATCH_END_REMINDER		17			//比赛结束倒计时
#define IDI_MATCH_END		18			//比赛结束
#define IDI_TIMER_LOAD_MATCH_CONFIG	19			//比赛配置读取
#define IDI_TIMER_SEND_FAKE_SERVERINFO	20		//send fake server info

bool compareMatchRank(tagMatchRankInfo & obj1, tagMatchRankInfo & obj2)
{
	if (obj1.lMatchScore > obj2.lMatchScore)
	{
		return true;
	}
	return false;
}

#define PRICE1 500000
#define PRICE2 200000
#define PRICE3 100000

__int64 lYesterdayScore = 50000000;
float fYesterdayPercent = 0.5f;
float fHighMultiple = 1.6f;
bool bHighTime[1440] = {0};
float fPearl1Percent = 0.1f;
float fPearl2Percent = 0.2f;

struct tagPearlInfo
{
	bool bHave;
	int nType;
	int nSendSecond;
};

std::vector<tagPearlInfo> vRand;
std::vector<tagPearlInfo> vPearlInfo;

struct tagUser1
{
	DWORD dwUserID;
	WORD wServerID;
};

std::vector<tagUser1> vUser1;

void setHighTime(int nBeginMinute, int nEndMinute)
{
	if (nBeginMinute >= _countof(bHighTime) || nEndMinute >= _countof(bHighTime))
	{
		return;
	}
	if (nBeginMinute >= nEndMinute)
	{
		return;
	}
	for (int i = nBeginMinute; i < nEndMinute; i++)
	{
		bHighTime[i] = true;
	}
}

void calcPearlCount()
{
	__int64 lUseScore = lYesterdayScore * fYesterdayPercent;

	int nPearl1Count = (lUseScore * fPearl1Percent) / PRICE1;
	int nPearl2Count = (lUseScore * fPearl2Percent) / PRICE2;
	int nPearl3Count = (lUseScore - nPearl1Count * PRICE1 - nPearl2Count * PRICE2) / PRICE3;
	int nTotalCount = nPearl1Count + nPearl2Count + nPearl3Count;

	for (int i = 0; i < nPearl1Count; i++)
	{
		tagPearlInfo PearlInfo = {0};
		PearlInfo.nType = 2;
		vRand.push_back(PearlInfo);
	}
	for (int i = 0; i < nPearl2Count; i++)
	{
		tagPearlInfo PearlInfo = {0};
		PearlInfo.nType = 1;
		vRand.push_back(PearlInfo);
	}
	for (int i = 0; i < nPearl3Count; i++)
	{
		tagPearlInfo PearlInfo = {0};
		PearlInfo.nType = 0;
		vRand.push_back(PearlInfo);
	}
	while (vRand.size() > 0)
	{
		int nIndex = rand() % vRand.size();
		vPearlInfo.push_back(vRand[nIndex]);
		vRand.erase(vRand.begin() + nIndex);
	}
}

void calcPearlTime()
{
	int nTotalCount = vPearlInfo.size();

	float fCountPerHour = (float)nTotalCount / 24;

	int nHighTimeCount = 0;
	int nLowTimeCount = 0;
	for (int i = 0; i < _countof(bHighTime); i++)
	{
		if (bHighTime[i])
		{
			nHighTimeCount++;
		}
		else
		{
			nLowTimeCount++;
		}
	}

	float fLowPearlCount = nTotalCount / (fHighMultiple * nHighTimeCount + nLowTimeCount);
	float fHighPearlCount = fLowPearlCount * fHighMultiple;

	float fHighSecond = (86400 / _countof(bHighTime)) / fHighPearlCount;
	float fLowSecond = (86400 / _countof(bHighTime)) / fLowPearlCount;

	float fTempSecond = 0;
	for (unsigned int i = 0; i < vPearlInfo.size(); i++)
	{
		int nIndex = fTempSecond / 60;
		float fAdd = 0.0f;
		if (bHighTime[nIndex])
		{
			fAdd = fHighSecond;
		}
		else
		{
			fAdd = fLowSecond;
		}
		fTempSecond += fAdd;
		if (fTempSecond > 86400)
		{
			break;
		}
		vPearlInfo[i].nSendSecond = fTempSecond;
	}
}

void InitPearlInfo()
{
	vPearlInfo.clear();
	calcPearlCount();

	setHighTime(0, 240);
	setHighTime(1260, 1439);

	calcPearlTime();

	SYSTEMTIME st;
	GetLocalTime(&st);
	int nCurrentSecond = st.wHour;
	nCurrentSecond *= 3600;
	nCurrentSecond += st.wMinute * 60 + st.wSecond;
	for (unsigned int i = 0; i < vPearlInfo.size(); i++)
	{
		tagPearlInfo & PearlInfo = vPearlInfo[i];
		if (PearlInfo.nSendSecond <= nCurrentSecond)
		{
			PearlInfo.bHave = false;
		}
		else
		{
			PearlInfo.bHave = true;
		}
	}
}

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_wCollectItem=INVALID_WORD;

	//设置变量
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	m_bInitLotteryInfo = false;
	m_nCurrentMatchID = 0;
	m_nDifferentTime = 0;
	m_vMatchData.clear();
	m_vMatchGameServerID.clear();

	// Initialize GameServerInfo
	m_vMatchGameServerID.resize(300);
	memset(m_IsRemind,0,sizeof(m_IsRemind));
	// Initialize the 3 dimension vector
	m_vMatchData.resize(300);  
	for (int i=0; i < 300; i ++ )
	{
		m_vMatchData[i].resize(100);
		//for (int j = 0; j < 100; j++)
		//{
		//	m_vMatchData[i][j].resize(8);
		//}
	}

	m_wUserNumPerGroup = 8; // test tmp set value 
	m_wGroupCnt = 0;
	m_CurrentFakeNameNum = 0;
	m_wAndroidFakeNameCnt = sizeof(g_pszFakeName)/sizeof(*g_pszFakeName);
	m_bIsGameBegin = false;
	printf ("the number of the Fake name is %d\n", m_wAndroidFakeNameCnt);

	// Fake server info time interval
	//m_wSendFakeInfoInterval = 30000;

	return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//生成话费数据
void CAttemperEngineSink::GenLotteryInfo(int nMaxSend, int nSendElapse, tagLotteryInfo & LotteryInfo)
{
	if (m_nLastSendElapse > 86400)
	{
		printf("86400+\n");
		return;
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	char szSerial[20];

	for (int i = 0; i < nMaxSend; i++)
	{
		tagLotteryInfo mm = {0};
		mm.nPrice = LotteryInfo.nPrice;
		mm.nMaxGet = LotteryInfo.nMaxGet;
		mm.nLessTime = LotteryInfo.nLessTime;
		mm.nPercent = LotteryInfo.nPercent;
		mm.nMinusStorage = LotteryInfo.nMinusStorage;
		mm.wServerID = LotteryInfo.wServerID;
		mm.nSendSecond = m_nLastSendElapse;

		m_nLastSendElapse += nSendElapse;

		sprintf(szSerial, "%02hu%02hu%08d%05d", st.wMonth, st.wDay, mm.nPrice, mm.nSendSecond);
		mm.nSerial = _atoi64(szSerial);

		//判断是否已存在
		bool bHave = false;
		for (unsigned int i = 0; i < m_vLotteryInfo.size(); i++)
		{
			if (m_vLotteryInfo[i].nSerial == mm.nSerial)
			{
				bHave = true;
				break;
			}
		}
		if (!bHave)
		{
			m_vLotteryInfo.push_back(mm);
		}
		else
		{
			printf("already exist\n");
		}
	}

	int nCurrentSecond = st.wHour;
	nCurrentSecond *= 3600;
	nCurrentSecond += st.wMinute * 60 + st.wSecond;

	for (unsigned int i = 0; i < m_vLotteryInfo.size(); i++)
	{
		tagLotteryInfo & LotteryInfo = m_vLotteryInfo[i];
		if (LotteryInfo.nSendSecond <= nCurrentSecond)
		{
			LotteryInfo.bHave = false;
		}
		else
		{
			LotteryInfo.bHave = true;
		}
	}
}

struct tagRandData
{
	WORD wServerID;
	int nLotteryCount;
};

//初始化话费数据
void CAttemperEngineSink::InitLotteryInfo()
{
	m_nLastSendElapse = 0;
	m_vLotteryInfo.clear();

	tagLotteryInfo LotteryInfo = {0};
	LotteryInfo.nMaxGet = 10000000;
	LotteryInfo.nLessTime = 0;
	LotteryInfo.nPercent = 100;
	LotteryInfo.nMinusStorage = 0;

	std::vector<tagRandData> vData;
	std::vector<tagRandData> vRandData;
	vData.clear();
	vRandData.clear();
	srand((unsigned int)time(NULL));

	//生成数据
	{
		tagRandData rd = {0};
		//用于新手房，3055个
		rd.wServerID = 309;
		for (int i = 0; i < 518; i++)
		{
			rd.nLotteryCount = 1;
			vData.push_back(rd);
			rd.nLotteryCount = 5;
			vData.push_back(rd);
			rd.nLotteryCount = 2;
			vData.push_back(rd);
			rd.nLotteryCount = 4;
			vData.push_back(rd);
		}
		for (int i = 0; i < 983; i++)
		{
			rd.nLotteryCount = 3;
			vData.push_back(rd);
		}

		//用于百炮房，3112个
		rd.wServerID = 315;
		for (int i = 0; i < 518; i++)
		{
			rd.nLotteryCount = 10;
			vData.push_back(rd);
			rd.nLotteryCount = 50;
			vData.push_back(rd);
			rd.nLotteryCount = 20;
			vData.push_back(rd);
			rd.nLotteryCount = 40;
			vData.push_back(rd);
		}
		for (int i = 0; i < 1040; i++)
		{
			rd.nLotteryCount = 30;
			vData.push_back(rd);
		}

		//用于千炮房，1334个
		rd.wServerID = 316;
		for (int i = 0; i < 222; i++)
		{
			rd.nLotteryCount = 50;
			vData.push_back(rd);
			rd.nLotteryCount = 100;
			vData.push_back(rd);
			rd.nLotteryCount = 60;
			vData.push_back(rd);
			rd.nLotteryCount = 90;
			vData.push_back(rd);
		}
		for (int i = 0; i < 223; i++)
		{
			rd.nLotteryCount = 70;
			vData.push_back(rd);
			rd.nLotteryCount = 80;
			vData.push_back(rd);
		}

		//用于万炮房，354个
		rd.wServerID = 317;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 41; j++)//100~500，12300（(40*41/2)*10+100*41）
			{
				rd.nLotteryCount = 100 + j * 10;
				vData.push_back(rd);
			}
		}
		for (int i = 0; i < 26; i++)
		{
			rd.nLotteryCount = 300;
			vData.push_back(rd);
		}
	}

	//打乱数据
	{
		do
		{
			int nCount = vData.size();
			int nIndex = rand() % nCount;
			vRandData.push_back(vData[nIndex]);
			vData.erase(vData.begin() + nIndex);
		} while (vData.size() > 0);
	}

	for (unsigned int i = 0; i < vRandData.size(); i++)
	{
		LotteryInfo.nPrice = vRandData[i].nLotteryCount;
		LotteryInfo.wServerID = vRandData[i].wServerID;
		GenLotteryInfo(1, 11, LotteryInfo);
	}

	printf("total is %d\n", m_vLotteryInfo.size());

	m_bInitLotteryInfo = true;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定参数
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

	InitLotteryInfo();

	m_pITimerEngine->SetTimer(IDI_LOTTERY_INFO,1000,TIMES_INFINITY,NULL);
	//m_pITimerEngine->SetTimer(IDI_PEARL,m_wSendFakeInfoInterval,TIMES_INFINITY,NULL);
	// fake GameServer info
	m_pITimerEngine->SetTimer(IDI_TIMER_SEND_FAKE_SERVERINFO,1000,TIMES_INFINITY,NULL);

	//read match info here.
	//max group, max number in each group
	m_pITimerEngine->SetTimer(IDI_READ_MATCH_CONFIG,60000,(DWORD)(-1),NULL);
	m_pITimerEngine->SetTimer(IDI_TIMER_CHECK_INTERVAL,1000,(DWORD)(-1),NULL);
	m_pITimerEngine->SetTimer(IDI_TIMER_LOAD_MATCH_CONFIG,3000,1,NULL);
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\GameMatchConfig.ini"),szPath);

	//读取配置
	TCHAR szServerName[MAX_PATH]=TEXT("");
	m_nMessageRemindTime1 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime1"),5,szConfigFileName);
	m_nMessageRemindTime2 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime2"),3,szConfigFileName);
	m_nMessageRemindTime3 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime3"),1,szConfigFileName);
	m_nMatchBeginRemindTime = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MatchBeginRemindTime"),5,szConfigFileName);
	m_nNewPlayerServerId = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("NewPlayerServerId"),309,szConfigFileName);
	m_nHundredServerId = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("HundredServerId"),315,szConfigFileName);
	m_nThousandServerId = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("ThousandServerId"),316,szConfigFileName);
	m_nTenThousandServerId = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("TenThousandServerId"),317,szConfigFileName);
	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//状态变量
	m_wCollectItem=INVALID_WORD;
	m_WaitCollectItemArray.RemoveAll();

	//设置变量
	m_pITimerEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	//删除数据
	SafeDeleteArray(m_pBindParameter);

	//设置组件
	m_GlobalInfoManager.ResetData();

	return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
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
	switch (dwTimerID)
	{
	case IDI_LOTTERY_INFO:
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			int nCurrentSecond = st.wHour;
			nCurrentSecond *= 3600;
			nCurrentSecond += st.wMinute * 60 + st.wSecond;

			if (st.wHour == 0 && st.wMinute == 0)
			{
				if (!m_bInitLotteryInfo)
				{
					InitLotteryInfo();
					m_vMatchRankInfo.clear();
				}
			}
			else
			{
				m_bInitLotteryInfo = false;
			}

			int nSelectIndex = -1;
			for (unsigned int i = 0; i < m_vLotteryInfo.size(); i++)
			{
				tagLotteryInfo & LotteryInfo = m_vLotteryInfo[i];
				if (!LotteryInfo.bHave) continue;

				//到达派送时间
				if (LotteryInfo.nSendSecond <= nCurrentSecond)
				{
					nSelectIndex = i;
					//printf("发奖时间 %d\n", nCurrentSecond);
					break;
				}
			}

			//有奖可发
			if (nSelectIndex >= 0)
			{
				m_vLotteryInfo[nSelectIndex].bHave = false;
				CMD_CS_S_LotteryInfo mm1 = {0};
				tagLotteryInfo & mm2 = m_vLotteryInfo[nSelectIndex];
				mm1.nPrice = mm2.nPrice;
				mm1.nMaxGet = mm2.nMaxGet;
				mm1.nLessTime = mm2.nLessTime;
				mm1.nPercent = mm2.nPercent;
				mm1.nMinusStorage = mm2.nMinusStorage;
				SendDataToGame(mm2.wServerID, MDM_CS_MANAGER_SERVICE, SUB_CS_S_LOTTERY_INFO, &mm1, sizeof(mm1));
			}

			return true;
		}
	case IDI_READ_MATCH_CONFIG:
		{
			//读取配置
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR szConfigFileName[MAX_PATH] = TEXT("");
			GetCurrentDirectory(sizeof(szPath),szPath);
			_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\GameMatchConfig.ini"),szPath);

			//读取配置
			TCHAR szServerName[MAX_PATH]=TEXT("");
			m_nMessageRemindTime1 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime1"),5,szConfigFileName);
			m_nMessageRemindTime2 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime2"),3,szConfigFileName);
			m_nMessageRemindTime3 = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MessageRemindTime3"),1,szConfigFileName);
			m_nMatchBeginRemindTime = GetPrivateProfileInt(TEXT("MatchConfig"),TEXT("MatchBeginRemindTime"),5,szConfigFileName);
		}
		break;
	case IDI_TIMER_CHECK_INTERVAL:
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			if (st.wHour==23&&st.wMinute==58&&st.wSecond==59)
			{
				//加载比赛配置
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_MATCH_CONFIG,0,NULL,0L);
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_GET_SQL_TIME,0,NULL,0);
			}
			tagMatchConfigItem sMatchConfigItem={0};
			GetNextMatchInfo(sMatchConfigItem);
			int nMatchStartTimeHour = sMatchConfigItem.nStartTime/100;
			int nMatchStartTimeMinute = sMatchConfigItem.nStartTime%100;

			int nMatchStartTime = 60 * ((sMatchConfigItem.nStartTime)/100 * 60 + (sMatchConfigItem.nStartTime)%100 ); //以秒为单位
			int nCurrentTime = 60*(st.wHour * 60 + st.wMinute)+ st.wSecond - m_nDifferentTime;
			int nDiffTime = nMatchStartTime - nCurrentTime;
			MatchNotification stMatchNotification = {0};
			stMatchNotification.nMatchID = sMatchConfigItem.nMatchNum;
			stMatchNotification.nMatchPeopleNum = sMatchConfigItem.nMatchUserCount;
			stMatchNotification.nMatchScore = sMatchConfigItem.nApplyCost;
			stMatchNotification.nMatchType = sMatchConfigItem.nMatchType;
			/*printf("差距时间=%d，RemindTime1=%d，RemindTime2=%d，RemindTime3=%d，是否提醒过1=%d,2=%d,3=%d\n",nDiffTime,m_nMessageRemindTime1*60,m_nMessageRemindTime2*60,m_nMessageRemindTime3*60,m_IsRemind[0],m_IsRemind[1],m_IsRemind[2]);*/
			if (nDiffTime>0&&nDiffTime<=(m_nMessageRemindTime1*60)&&m_IsRemind[0]==0)
			{
				m_nCurrentMatchID = sMatchConfigItem.nMatchNum;
				printf("当前游戏场次=%d\n",m_nCurrentMatchID);
				//发送提示信息
				printf("5分钟检测\n");
				m_IsRemind[0]=1;
				_sntprintf(stMatchNotification.szNotification,CountArray(stMatchNotification.szNotification),TEXT("%s还有%d分钟开始"),sMatchConfigItem.szMatchName,m_nMessageRemindTime1);
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION,&stMatchNotification,sizeof(stMatchNotification),0L);
			}
			else if (nDiffTime>0&&nDiffTime<=(m_nMessageRemindTime2*60)&&m_IsRemind[1]==0)
			{
				m_nCurrentMatchID = sMatchConfigItem.nMatchNum;
				printf("当前游戏场次=%d\n",m_nCurrentMatchID);
				//发送提示信息
				printf("3分钟检测\n");
				m_IsRemind[1]=1;
				_sntprintf(stMatchNotification.szNotification,CountArray(stMatchNotification.szNotification),TEXT("%s还有%d分钟开始"),sMatchConfigItem.szMatchName,m_nMessageRemindTime2);
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION,&stMatchNotification,sizeof(stMatchNotification),0L);
			}
			else if (nDiffTime>0&&nDiffTime<=(m_nMessageRemindTime3*60)&&m_IsRemind[2]==0)
			{
				m_nCurrentMatchID = sMatchConfigItem.nMatchNum;
				printf("当前游戏场次=%d\n",m_nCurrentMatchID);
				//发送提示信息
				printf("1分钟检测\n");
				memset(m_IsRemind,0,sizeof(m_IsRemind));
				_sntprintf(stMatchNotification.szNotification,CountArray(stMatchNotification.szNotification),TEXT("%s还有%d分钟开始"),sMatchConfigItem.szMatchName,m_nMessageRemindTime3);
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION,&stMatchNotification,sizeof(stMatchNotification),0L);

				m_pITimerEngine->SetTimer(IDI_MATCH_BEGIN_REMINDER,55000,1,NULL);
				m_pITimerEngine->KillTimer(IDI_TIMER_CHECK_INTERVAL);
			}

		}
		break;
	case IDI_MATCH_BEGIN_REMINDER:
		{
				//发送提示信息
				CMD_GPO_MatchTimeNotify stMatchNotification={0};
				stMatchNotification.cbMatchStatus = 0;
				stMatchNotification.nSecond = 5;
				printf("发送5秒倒计时信息房间数量=%d\n",m_vMatchGameServerID.size());
				tagMatchConfigItem stMatchConfigItem={0};
				GetCurrentMatchInfo(m_nCurrentMatchID,stMatchConfigItem);
				char szPrize[128];
				sprintf(szPrize,"赢%s即将开始",stMatchConfigItem.szMatchName);
				lstrcpyn(stMatchNotification.szTitle,szPrize,sizeof(stMatchNotification.szTitle));

				for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID-1].size(); i++)
				{
					printf("游戏房间id=%d\n",m_vMatchGameServerID[m_nCurrentMatchID-1][i]);
					SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID-1][i],MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION_COUNTDOWN,&stMatchNotification, sizeof(CMD_GPO_MatchTimeNotify));
				}
			m_pITimerEngine->SetTimer(IDI_MATCH_BEGIN,5100,1,NULL);
		}
		break;
	case IDI_MATCH_BEGIN:
			{
				//发送提示信息
				CMD_CS_S_MatchBegin stMatchBegin = {0};

				// Game begin
				m_bIsGameBegin = true;

				//Set Current Group Count
				for (int i = 0; i < m_vMatchData[m_nCurrentMatchID - 1].size(); i++)
				{
					if (m_vMatchData[m_nCurrentMatchID - 1][i].size() > 0)
					{
						m_wGroupCnt++;
					}
				}

				// Get Current Time
				time_t rawtime;
				struct tm * timeinfo;
				TCHAR buffer[80];

				time (&rawtime);
				timeinfo = localtime(&rawtime);

				// fill the match info structure
				tagMatchConfigItem stMatchConfigItem={0};
				GetCurrentMatchInfo(m_nCurrentMatchID,stMatchConfigItem);

				// init the UserCnt in Group
				m_wUserNumPerGroup = stMatchConfigItem.nMatchUserCount;

				// Set the Current MatchID 
				m_nCurrentMatchID = stMatchConfigItem.nMatchNum;
				stMatchBegin.nMatchID = stMatchConfigItem.nMatchNum;
				lstrcpyn(stMatchBegin.szMatchName,stMatchConfigItem.szMatchName,CountArray(stMatchBegin.szMatchName));
				strftime(stMatchBegin.szMatchDate,80,"%Y-%m-%d",timeinfo);
				printf("比赛房间数目=%d\n",m_vMatchGameServerID[m_nCurrentMatchID - 1].size());
				for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID - 1].size(); i++)
				{
					printf("房间id=%d\n",m_vMatchGameServerID[m_nCurrentMatchID-1][i]);
					SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID-1][i],MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_BEGIN,&stMatchBegin, sizeof(CMD_CS_S_MatchBegin));
				}
				int nMatchTime = ((stMatchConfigItem.nEndTime/100)*60+stMatchConfigItem.nEndTime%100)-((stMatchConfigItem.nStartTime/100)*60+stMatchConfigItem.nStartTime%100);
				m_pITimerEngine->SetTimer(IDI_MATCH_END_REMINDER,nMatchTime*60000-5000,1,NULL);
				time_t now = time(NULL);
				strftime(m_szMatchBeginTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
				//添加假数据,先获得最后一组不够的队伍
				int nEndIdx = m_wGroupCnt;
				if (nEndIdx>=1)
				{
					int nNotEnoughNum = stMatchConfigItem.nMatchUserCount-m_vMatchData[m_nCurrentMatchID-1][nEndIdx-1].size();
					printf("the size of the end group is %d, NotEnoughNum is %d\n", nEndIdx, nNotEnoughNum);
					tagMatchItem stMatchItem = {0};
					if (nNotEnoughNum > 0)
					{
						m_CurrentFakeNameNum = rand()%m_wAndroidFakeNameCnt;
						for (int i = m_vMatchData[m_nCurrentMatchID-1][nEndIdx-1].size(); i < stMatchConfigItem.nMatchUserCount; ++i)
						{
							stMatchItem.dwUserID = 0;
							stMatchItem.wServerID = m_vMatchData[m_nCurrentMatchID-1][nEndIdx-1][0].wServerID;
							stMatchItem.bIsRobot = true;
							m_CurrentFakeNameNum %= m_wAndroidFakeNameCnt;
							lstrcpyn(stMatchItem.szNickName,g_pszFakeName[m_CurrentFakeNameNum],CountArray(stMatchItem.szNickName));
							m_vMatchData[m_nCurrentMatchID-1][nEndIdx-1].push_back(stMatchItem);
							m_CurrentFakeNameNum ++;

							printf ("fake nickname %s inserted in \n", m_vMatchData[m_nCurrentMatchID-1][nEndIdx-1][i].szNickName);
						}
					}
			
			}
		}
		break;
	case IDI_MATCH_END_REMINDER:
		{
			if (m_nCurrentMatchID == 0)
			{
				printf ("Invalid Current Match ID\n");
				return false;
			}

			CMD_GPO_MatchTimeNotify stMatchNotification={0};
			stMatchNotification.cbMatchStatus = 1;
			stMatchNotification.nSecond = 5;
			tagMatchConfigItem stMatchConfigItem={0};
			GetCurrentMatchInfo(m_nCurrentMatchID,stMatchConfigItem);
			char szPrize[128];
			sprintf(szPrize,"赢%s即将结束",stMatchConfigItem.szMatchName);
			lstrcpyn(stMatchNotification.szTitle,szPrize,sizeof(stMatchNotification.szTitle));
			for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID-1].size(); i++)
			{
				printf("游戏房间id=%d\n",m_vMatchGameServerID[m_nCurrentMatchID-1][i]);
				SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID-1][i],MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION_COUNTDOWN,&stMatchNotification, sizeof(CMD_GPO_MatchTimeNotify));
			}

			m_pITimerEngine->SetTimer(IDI_MATCH_END,5000,1,NULL);
		}
		break;
	case IDI_MATCH_END:
		{
			if (m_nCurrentMatchID == 0)
			{
				printf ("Invalid Current Match ID\n");
				return false;
			}

			// Game End
			m_bIsGameBegin = false;

			// assume use the same info here	
			CMD_CS_S_MatchEnd stMatchEnd = {0};
			WORD wPacketSize=0;
			BYTE cbBuffer[204800];
			stMatchResult * pMatchResultItem=NULL;
			time_t now = time(NULL);
			for (int i = 0; i < m_vMatchData[m_nCurrentMatchID - 1].size(); ++i)
			{
				std::sort (m_vMatchData[m_nCurrentMatchID - 1][i].begin(), m_vMatchData[m_nCurrentMatchID - 1][i].end(),Sort_Match_Score());
				for(int j = 0; j < m_vMatchData[m_nCurrentMatchID - 1][i].size(); ++j)
				{
					m_vMatchData[m_nCurrentMatchID - 1][i][j].cbRank = j + 1;
					printf("%d排名=%d\n",j,m_vMatchData[m_nCurrentMatchID - 1][i][j].cbRank);
					if (m_vMatchData[m_nCurrentMatchID - 1][i][j].bIsRobot)
					{
						continue;
					}
					pMatchResultItem=(stMatchResult *)(cbBuffer+wPacketSize);
					pMatchResultItem->dwUserID = m_vMatchData[m_nCurrentMatchID - 1][i][j].dwUserID;
					pMatchResultItem->lUserScore = m_vMatchData[m_nCurrentMatchID - 1][i][j].lMatchScore;
					pMatchResultItem->nMatchRank = m_vMatchData[m_nCurrentMatchID - 1][i][j].cbRank;
					pMatchResultItem->nServerId = m_vMatchData[m_nCurrentMatchID - 1][i][j].wServerID;
					pMatchResultItem->nIsComplete = 1;
					pMatchResultItem->nGroupId = m_vMatchData[m_nCurrentMatchID - 1][i][j].wGroupID;
					pMatchResultItem->nIsRobot = m_vMatchData[m_nCurrentMatchID - 1][i][j].bIsRobot;
					pMatchResultItem->nMatchNum = m_vMatchData[m_nCurrentMatchID - 1][i][j].cbMatchNum;
					pMatchResultItem->nMatchType = m_vMatchData[m_nCurrentMatchID - 1][i][j].cbMatchType;
					if (m_vMatchData[m_nCurrentMatchID - 1][i][j].lMatchScore==0)
					{
						pMatchResultItem->nIsGet = 2;//2为无奖励
					}
					else
					{
						pMatchResultItem->nIsGet = 0;
					}

					lstrcpyn(pMatchResultItem->szBeginTime,m_szMatchBeginTime,sizeof(pMatchResultItem->szBeginTime));
					strftime(pMatchResultItem->szEndTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

					wPacketSize+=sizeof(stMatchResult);
				}
			}
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_SAVE_MATCH_RESULT,0,cbBuffer,wPacketSize);
			tagMatchConfigItem stMatchConfigItem={0};
			GetCurrentMatchInfo(m_nCurrentMatchID,stMatchConfigItem);
			printf("发送比赛结束消息，比赛类型=%d",stMatchConfigItem.nMatchType);
			SendMatchEndInfoToServer(stMatchConfigItem.nMatchType);
			for (int i = 0; i < m_vMatchData[m_nCurrentMatchID - 1].size(); ++i)
			{
				m_vMatchData[m_nCurrentMatchID - 1][i].clear();
			}
			// Send msg to GameServer
			m_wGroupCnt = 0;
			m_nCurrentMatchID = 0;
			m_CurrentFakeNameNum = 0;
			m_pITimerEngine->SetTimer(IDI_TIMER_CHECK_INTERVAL,1000,(DWORD)(-1),NULL);
		}
		break;
	case IDI_TIMER_LOAD_MATCH_CONFIG:
		{
			//加载比赛时间配置信息
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_MATCH_CONFIG,0,NULL,0);
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_GET_SQL_TIME,0,NULL,0);
		}
		break;
	//case IDI_PEARL:
	//	{
	//		SYSTEMTIME st;
	//		GetLocalTime(&st);
	//		int nCurrentSecond = st.wHour;
	//		nCurrentSecond *= 3600;
	//		nCurrentSecond += st.wMinute * 60 + st.wSecond;

	//		int nSelectIndex = -1;
	//		for (unsigned int i = 0; i < vPearlInfo.size(); i++)
	//		{
	//			tagPearlInfo & PearlInfo = vPearlInfo[i];
	//			if (!PearlInfo.bHave) continue;

	//			if (PearlInfo.nSendSecond <= nCurrentSecond)
	//			{
	//				nSelectIndex = i;
	//				vPearlInfo[nSelectIndex].bHave = false;
	//				break;
	//			}
	//		}

	//		vUser1.clear();
	//		POSITION Position=NULL;
	//		do
	//		{
	//			CGlobalUserItem * pGlobalUserItem = m_GlobalInfoManager.EnumUserItem(Position);
	//			if (pGlobalUserItem != NULL)
	//			{
	//				if (!pGlobalUserItem->IsAndroid())
	//				{
	//					tagUser1 User1 = {0};
	//					User1.dwUserID = pGlobalUserItem->GetUserID();
	//					if (pGlobalUserItem->m_GlobalServerItemArray.GetCount() > 0)
	//					{
	//						User1.wServerID = pGlobalUserItem->m_GlobalServerItemArray[0]->GetServerID();
	//					}
	//					if (User1.wServerID != 309)
	//					{
	//						vUser1.push_back(User1);
	//					}
	//				}
	//			}
	//		} while (Position != NULL);

	//		if (nSelectIndex >= 0 && vUser1.size() > 0)
	//		{
	//			int nIndex = rand() % vUser1.size();
	//			tagUser1 & User1 = vUser1[nIndex];

	//			CMD_CS_S_PearlInfo mm1 = {0};
	//			tagPearlInfo & mm2 = vPearlInfo[nSelectIndex];
	//			mm1.dwUserID = User1.dwUserID;
	//			mm1.nType = mm2.nType;
	//			SendDataToGame(User1.wServerID, MDM_CS_MANAGER_SERVICE, SUB_CS_S_PEARL_INFO, &mm1, sizeof(mm1));
	//		}

	//		return true;
	//	}
	case IDI_TIMER_SEND_FAKE_SERVERINFO:
		{

			return true;
		}
	}

	return false;
}

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//获取索引
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//设置变量
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_REGISTER:		//服务注册
		{
			return OnTCPNetworkMainRegister(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_SERVICE_INFO:	//服务信息
		{
			return OnTCPNetworkMainServiceInfo(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_USER_COLLECT:	//用户命令
		{
			return OnTCPNetworkMainUserCollect(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_REMOTE_SERVICE:	//远程服务
		{
			return OnTCPNetworkMainRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_MANAGER_SERVICE: //管理服务
		{
			return OnTCPNetworkMainManagerService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//游戏服务
	if (pBindParameter->ServiceKind==ServiceKind_Game)
	{
		//变量定义
		WORD wBindIndex=LOWORD(dwSocketID);

		//汇总处理
		if (wBindIndex==m_wCollectItem)
		{
			//设置变量
			m_wCollectItem=INVALID_WORD;

			//汇总切换
			if (m_WaitCollectItemArray.GetCount()>0)
			{
				//提取变量
				INT_PTR nWaitCount=m_WaitCollectItemArray.GetCount();
				m_wCollectItem=m_WaitCollectItemArray[nWaitCount-1];

				//删除数组
				m_WaitCollectItemArray.RemoveAt(nWaitCount-1);

				//发送消息
				DWORD dwSocketID=(m_pBindParameter+m_wCollectItem)->dwSocketID;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}
		}
		else
		{
			//删除等待
			for (INT_PTR i=0;i<m_WaitCollectItemArray.GetCount();i++)
			{
				if (wBindIndex==m_WaitCollectItemArray[i])
				{
					m_WaitCollectItemArray.RemoveAt(i);
					break;
				}
			}
		}

		//变量定义
		CMD_CS_S_ServerRemove ServerRemove;
		ZeroMemory(&ServerRemove,sizeof(ServerRemove));

		//删除通知
		ServerRemove.wServerID=pBindParameter->wServiceID;
		m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_REMOVE,&ServerRemove,sizeof(ServerRemove),0L);

		//注销房间
		m_GlobalInfoManager.DeleteServerItem(pBindParameter->wServiceID);
	}

	//广场服务
	if (pBindParameter->ServiceKind==ServiceKind_Plaza)
	{
		//变量定义
		WORD wBindIndex=LOWORD(dwSocketID);
		tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

		//注销房间
		m_GlobalInfoManager.DeletePlazaItem(pBindParameter->wServiceID);
	}

	//清除信息
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));

	return false;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GC_INSERT_MATCH_SIGNUP_USER:			//比赛报名结果
		{
			//效验数据
			ASSERT(wDataSize==sizeof(DBO_GC_Insert_Match_SignUp_User));
			if (wDataSize!=sizeof(DBO_GC_Insert_Match_SignUp_User)) return false;

			bool bIsInsertSuc = false;
			WORD wGroupID = 0;
			DBO_GC_Insert_Match_SignUp_User * pInsertMatchSignUpUser = (DBO_GC_Insert_Match_SignUp_User *)pData;

			if (pInsertMatchSignUpUser->bIsSignUpSuc == true)
			{
				printf("size of matchdata %d\n", m_vMatchData[pInsertMatchSignUpUser->nMatchID - 1].size());
				char szMsg[100];
				sprintf( szMsg, "用户id=%d，报名场次=%d,这场当前人数=%d,报名费=%d\n",pInsertMatchSignUpUser->dwUserID,pInsertMatchSignUpUser->nMatchID,m_vMatchData[pInsertMatchSignUpUser->nMatchID - 1].size(),pInsertMatchSignUpUser->wEnrollmentFee);
				CString strTime(szMsg);
				OnRecord(strTime);
				for (unsigned int i = 0; i < m_vMatchData[pInsertMatchSignUpUser->nMatchID - 1].size(); i++)
				{
					printf("MatchID = %d, size of Current Id %d\n", pInsertMatchSignUpUser->nMatchID, m_vMatchData[pInsertMatchSignUpUser->nMatchID - 1][i].size());
					if(pInsertMatchSignUpUser->nMatchID != 0 && (m_vMatchData[pInsertMatchSignUpUser->nMatchID - 1][i].size() < m_wUserNumPerGroup))
					{
						wGroupID = i + 1;

						tagMatchItem MatchSignUp = {0};
						MatchSignUp.dwUserID = pInsertMatchSignUpUser->dwUserID;
						MatchSignUp.wServerID = pInsertMatchSignUpUser->wServerID;
						MatchSignUp.bIsRobot = false;
						MatchSignUp.cbMatchNum = pInsertMatchSignUpUser->nMatchID;
						MatchSignUp.cbMatchType = pInsertMatchSignUpUser->nMatchType;
						MatchSignUp.wGroupID = wGroupID;
						lstrcpyn(MatchSignUp.szNickName,pInsertMatchSignUpUser->szNickName,sizeof(MatchSignUp.szNickName));
						m_vMatchData[pInsertMatchSignUpUser->nMatchID - 1][i].push_back(MatchSignUp);

						// save the server id which has Match User
						if(std::find(m_vMatchGameServerID[pInsertMatchSignUpUser->nMatchID - 1].begin(), m_vMatchGameServerID[pInsertMatchSignUpUser->nMatchID - 1].end(), MatchSignUp.wServerID) == m_vMatchGameServerID[pInsertMatchSignUpUser->nMatchID - 1].end()) 
						{
							sprintf( szMsg, "insert Group ID is %d, the size of matchGameServerID %d\n",wGroupID, m_vMatchGameServerID[pInsertMatchSignUpUser->nMatchID - 1].size());
							CString strTime2(szMsg);
							OnRecord(strTime2);
							m_vMatchGameServerID[pInsertMatchSignUpUser->nMatchID - 1].push_back(MatchSignUp.wServerID);
						} 

						bIsInsertSuc = true;

						break;
					}
				}
			}

			// Send Sign Up Res
			CMD_CS_S_MatchSignUpRes stMatchSignUpRes = {0};
			stMatchSignUpRes.dwUserID = pInsertMatchSignUpUser->dwUserID;
			stMatchSignUpRes.wGroupID = wGroupID;
			stMatchSignUpRes.nMatchID = pInsertMatchSignUpUser->nMatchID;
			stMatchSignUpRes.bMatchStatus = pInsertMatchSignUpUser->bMatchStatus;
			stMatchSignUpRes.bIsInGame = pInsertMatchSignUpUser->bIsInGame;
			stMatchSignUpRes.wEnrollmentFee = pInsertMatchSignUpUser->wEnrollmentFee;
			lstrcpyn(stMatchSignUpRes.szDescription,pInsertMatchSignUpUser->szDescription,sizeof(stMatchSignUpRes.szDescription));

			//printf("Match sign up res Userid = %lld, wGroup = %ld, MatchID = %d, MatchStatus = %d, NickName = %s\n", stMatchSignUpRes.dwUserID , stMatchSignUpRes.wGroupID, stMatchSignUpRes.nMatchID, stMatchSignUpRes.bMatchStatus, stMatchSignUpRes.szDescription);
			//printf("Match sign up res Userid = %ld, fee is %d\n", stMatchSignUpRes.dwUserID, pInsertMatchSignUpUser->wEnrollmentFee);

			if (bIsInsertSuc == true && pInsertMatchSignUpUser->bIsSignUpSuc == true)
			{
				stMatchSignUpRes.bIsSignUpSuc = true;
			}
			else
			{
				stMatchSignUpRes.bIsSignUpSuc = false;
			}

			SendDataToGame(pInsertMatchSignUpUser->wServerID, MDM_CS_MANAGER_SERVICE, SUB_CS_S_MATCH_SIGNUP_RES, &stMatchSignUpRes, sizeof(stMatchSignUpRes));

			return true;
		}
	case DBO_LOAD_MATCH_CONFIG:
		{
			ASSERT(wDataSize%sizeof(DBO_Load_Match_Config_Item)==0);
			if (wDataSize%sizeof(DBO_Load_Match_Config_Item)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(DBO_Load_Match_Config_Item);
			DBO_Load_Match_Config_Item * pGameItem=(DBO_Load_Match_Config_Item *)pData;
			m_MatchConfigItemList.clear();
			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				//变量定义
				tagMatchConfigItem MatchConfigItem;
				ZeroMemory(&MatchConfigItem,sizeof(MatchConfigItem));

				//构造数据
				MatchConfigItem.nStartTime = pGameItem->nStartTime;
				MatchConfigItem.nEndTime = pGameItem->nEndTime;
				MatchConfigItem.nApplyCost = pGameItem->nApplyCost;
				MatchConfigItem.nGameTime = pGameItem->nGameTime;
				MatchConfigItem.nMatchNum = pGameItem->nMatchNum;
				MatchConfigItem.nMatchUserCount = pGameItem->nMatchUserCount;
				MatchConfigItem.nMatchType = pGameItem->nMatchType;
				lstrcpyn(MatchConfigItem.szMatchName,pGameItem->szMatchName,CountArray(MatchConfigItem.szMatchName));
				printf("比赛开始时间%d,比赛类型=%d\n",MatchConfigItem.nStartTime,MatchConfigItem.nMatchType);
				//插入列表
				m_MatchConfigItemList.push_back(MatchConfigItem);
				pGameItem++;
			}
			return true;
		}
	case DBO_GR_GET_SQL_TIME:
		{
			ASSERT(wDataSize==sizeof(DBO_Get_Sql_Time));
			if (wDataSize!=sizeof(DBO_Get_Sql_Time)) return false;

			//变量定义
			DBO_Get_Sql_Time * pSqlTime=(DBO_Get_Sql_Time *)pData;
			time_t SqlTime = StringToDatetime(pSqlTime->szTime);
			time_t now = time(NULL);
			m_nDifferentTime = now - SqlTime;
			return true;
		}
	}

	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	return false;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	return true;
}

//注册服务
bool CAttemperEngineSink::OnTCPNetworkMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_REGISTER_PLAZA:	//注册广场
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_CS_C_RegisterPlaza));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterPlaza)) return false;

			//消息定义
			CMD_CS_C_RegisterPlaza * pRegisterPlaza=(CMD_CS_C_RegisterPlaza *)pData;

			//有效判断
			if ((pRegisterPlaza->szServerName[0]==0)||(pRegisterPlaza->szServerAddr[0]==0))
			{
				//变量定义
				CMD_CS_S_RegisterFailure RegisterFailure;
				ZeroMemory(&RegisterFailure,sizeof(RegisterFailure));

				//设置变量
				RegisterFailure.lErrorCode=0L;
				lstrcpyn(RegisterFailure.szDescribeString,TEXT("服务器注册失败，“服务地址”与“服务器名”不合法！"),CountArray(RegisterFailure.szDescribeString));

				//发送消息
				WORD wStringSize=CountStringBuffer(RegisterFailure.szDescribeString);
				WORD wSendSize=sizeof(RegisterFailure)-sizeof(RegisterFailure.szDescribeString)+wStringSize;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REGISTER,SUB_CS_S_REGISTER_FAILURE,&RegisterFailure,wSendSize);

				//中断网络
				m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

				return true;
			}

			//设置绑定
			WORD wBindIndex=LOWORD(dwSocketID);
			(m_pBindParameter+wBindIndex)->wServiceID=wBindIndex;
			(m_pBindParameter+wBindIndex)->ServiceKind=ServiceKind_Plaza;

			//变量定义
			tagGamePlaza GamePlaza;
			ZeroMemory(&GamePlaza,sizeof(GamePlaza));

			//构造数据
			GamePlaza.wPlazaID=wBindIndex;
			lstrcpyn(GamePlaza.szServerName,pRegisterPlaza->szServerName,CountArray(GamePlaza.szServerName));
			lstrcpyn(GamePlaza.szServerAddr,pRegisterPlaza->szServerAddr,CountArray(GamePlaza.szServerAddr));

			//注册房间
			m_GlobalInfoManager.ActivePlazaItem(wBindIndex,GamePlaza);

			//发送列表
			SendServerListItem(dwSocketID);

			//群发设置
			m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true,0L);

			return true;
		}
	case SUB_CS_C_REGISTER_SERVER:	//注册房间
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_CS_C_RegisterServer));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterServer)) return false;

			//消息定义
			CMD_CS_C_RegisterServer * pRegisterServer=(CMD_CS_C_RegisterServer *)pData;

			//查找房间
			if (m_GlobalInfoManager.SearchServerItem(pRegisterServer->wServerID)!=NULL)
			{
				//变量定义
				CMD_CS_S_RegisterFailure RegisterFailure;
				ZeroMemory(&RegisterFailure,sizeof(RegisterFailure));

				//设置变量
				RegisterFailure.lErrorCode=0L;
				lstrcpyn(RegisterFailure.szDescribeString,TEXT("已经存在相同标识的游戏房间服务，房间服务注册失败"),CountArray(RegisterFailure.szDescribeString));

				//发送消息
				WORD wStringSize=CountStringBuffer(RegisterFailure.szDescribeString);
				WORD wSendSize=sizeof(RegisterFailure)-sizeof(RegisterFailure.szDescribeString)+wStringSize;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REGISTER,SUB_CS_S_REGISTER_FAILURE,&RegisterFailure,wSendSize);

				//中断网络
				m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

				return true;
			}

			//设置绑定
			WORD wBindIndex=LOWORD(dwSocketID);
			(m_pBindParameter+wBindIndex)->ServiceKind=ServiceKind_Game;
			(m_pBindParameter+wBindIndex)->wServiceID=pRegisterServer->wServerID;

			//变量定义
			tagGameServer GameServer;
			ZeroMemory(&GameServer,sizeof(GameServer));

			//构造数据
			GameServer.wKindID=pRegisterServer->wKindID;
			GameServer.wNodeID=pRegisterServer->wNodeID;
			GameServer.wSortID=pRegisterServer->wSortID;
			GameServer.wServerID=pRegisterServer->wServerID;
			GameServer.wServerPort=pRegisterServer->wServerPort;
			GameServer.dwOnLineCount=pRegisterServer->dwOnLineCount;
			GameServer.dwFullCount=pRegisterServer->dwFullCount;
			GameServer.lMinEnterScore=pRegisterServer->lMinEnterScore;
			lstrcpyn(GameServer.szServerName,pRegisterServer->szServerName,CountArray(GameServer.szServerName));
			lstrcpyn(GameServer.szServerAddr,pRegisterServer->szServerAddr,CountArray(GameServer.szServerAddr));

			//注册房间
			m_GlobalInfoManager.ActiveServerItem(wBindIndex,GameServer);

			//群发房间
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,&GameServer,sizeof(GameServer),0L);

			//发送列表
			SendServerListItem(dwSocketID);

			//群发设置
			m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true,0L);

			//汇总通知
			if (m_wCollectItem==INVALID_WORD)
			{
				m_wCollectItem=wBindIndex;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}
			else m_WaitCollectItemArray.Add(wBindIndex);

			return true;
		}
	}

	return false;
}

//服务状态
bool CAttemperEngineSink::OnTCPNetworkMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SERVER_ONLINE:	//房间人数
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_CS_C_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_C_ServerOnLine)) return false;

			//消息定义
			CMD_CS_C_ServerOnLine * pServerOnLine=(CMD_CS_C_ServerOnLine *)pData;

			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//查找房间
			WORD wServerID=pBindParameter->wServiceID;
			CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(wServerID);

			//设置人数
			if (pGlobalServerItem!=NULL)
			{
				//变量定义
				CMD_CS_S_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//设置变量
				pGlobalServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;

				//设置变量
				ServerOnLine.wServerID=wServerID;
				ServerOnLine.dwOnLineCount=pServerOnLine->dwOnLineCount;

				//发送通知
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine),0L);
			}

			return true;
		}
	case SUB_CS_C_SERVER_MODIFY:	//房间修改
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_CS_C_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_C_ServerModify)) return false;

			//消息定义
			CMD_CS_C_ServerModify * pServerModify=(CMD_CS_C_ServerModify *)pData;

			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//查找房间
			ASSERT(m_GlobalInfoManager.SearchServerItem(pBindParameter->wServiceID)!=NULL);
			CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(pBindParameter->wServiceID);

			//房间修改
			if (pGlobalServerItem!=NULL)
			{
				//设置变量
				pGlobalServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGlobalServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGlobalServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGlobalServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGlobalServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGlobalServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGlobalServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGlobalServerItem->m_GameServer.szServerName));
				lstrcpyn(pGlobalServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGlobalServerItem->m_GameServer.szServerAddr));

				//变量定义
				CMD_CS_S_ServerModify ServerModify;
				ZeroMemory(&ServerModify,sizeof(ServerModify));

				//设置变量
				ServerModify.wKindID=pServerModify->wKindID;
				ServerModify.wSortID=pServerModify->wSortID;
				ServerModify.wNodeID=pServerModify->wNodeID;
				ServerModify.wServerID=pBindParameter->wServiceID;
				ServerModify.wServerPort=pServerModify->wServerPort;
				ServerModify.dwOnLineCount=pServerModify->dwOnLineCount;
				ServerModify.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(ServerModify.szServerAddr,pServerModify->szServerAddr,CountArray(ServerModify.szServerAddr));
				lstrcpyn(ServerModify.szServerName,pServerModify->szServerName,CountArray(ServerModify.szServerName));

				//发送通知
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_MODIFY,&ServerModify,sizeof(ServerModify),0L);
			}

			return true;
		}
	}

	return false;
}

//用户处理
bool CAttemperEngineSink::OnTCPNetworkMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_USER_ENTER:		//用户进入
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_CS_C_UserEnter));
			//if (wDataSize!=sizeof(CMD_CS_C_UserEnter)) return false;
			if (wDataSize!=sizeof(CMD_CS_C_UserEnter)) return true;

			//消息处理
			CMD_CS_C_UserEnter * pUserEnter=(CMD_CS_C_UserEnter *)pData;
			pUserEnter->szNickName[CountArray(pUserEnter->szNickName)-1]=0;

			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//变量定义
			tagGlobalUserInfo GlobalUserInfo;
			ZeroMemory(&GlobalUserInfo,sizeof(GlobalUserInfo));

			//设置变量
			GlobalUserInfo.dwUserID=pUserEnter->dwUserID;
			GlobalUserInfo.dwGameID=pUserEnter->dwGameID;
			lstrcpyn(GlobalUserInfo.szNickName,pUserEnter->szNickName,CountArray(GlobalUserInfo.szNickName));

			//辅助信息
			GlobalUserInfo.cbGender=pUserEnter->cbGender;
			GlobalUserInfo.cbMemberOrder=pUserEnter->cbMemberOrder;
			GlobalUserInfo.cbMasterOrder=pUserEnter->cbMasterOrder;
			GlobalUserInfo.cbAndroid=pUserEnter->cbAndroid;

			//激活用户
			m_GlobalInfoManager.ActiveUserItem(GlobalUserInfo,pBindParameter->wServiceID);

			return true;
		}
	case SUB_CS_C_USER_LEAVE:		//用户离开
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_CS_C_UserLeave));
			if (wDataSize!=sizeof(CMD_CS_C_UserLeave)) return false;

			//消息处理
			CMD_CS_C_UserLeave * pUserLeave=(CMD_CS_C_UserLeave *)pData;

			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//删除用户
			m_GlobalInfoManager.DeleteUserItem(pUserLeave->dwUserID,pBindParameter->wServiceID);

			return true;
		}
	case SUB_CS_C_USER_FINISH:		//用户完成
		{
			//获取参数
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//连接效验
			ASSERT((m_wCollectItem==wBindIndex)&&(pBindParameter->ServiceKind==ServiceKind_Game));
			if ((m_wCollectItem!=wBindIndex)||(pBindParameter->ServiceKind!=ServiceKind_Game)) return false;

			//设置变量
			m_wCollectItem=INVALID_WORD;

			//汇总切换
			if (m_WaitCollectItemArray.GetCount()>0)
			{
				//切换汇总
				INT_PTR nWaitCount=m_WaitCollectItemArray.GetCount();
				m_wCollectItem=m_WaitCollectItemArray[nWaitCount-1];

				//删除数组
				m_WaitCollectItemArray.RemoveAt(nWaitCount-1);

				//发送消息
				DWORD dwSocketID=(m_pBindParameter+m_wCollectItem)->dwSocketID;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}

			return true;
		}
	}

	return false;
}

//远程服务
bool CAttemperEngineSink::OnTCPNetworkMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SEARCH_CORRESPOND:	//协调查找
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_CS_C_SearchCorrespond)) return false;

			//处理消息
			CMD_CS_C_SearchCorrespond * pSearchCorrespond=(CMD_CS_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//变量定义
			CMD_CS_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//结果用户
			CGlobalUserItem * ResultUserItem[2];
			ZeroMemory(ResultUserItem,sizeof(ResultUserItem));

			//设置变量
			SearchCorrespond.dwSocketID=pSearchCorrespond->dwSocketID;
			SearchCorrespond.dwClientAddr=pSearchCorrespond->dwClientAddr;

			//查找用户
			if (pSearchCorrespond->dwGameID!=0L)
			{
				ResultUserItem[0]=m_GlobalInfoManager.SearchUserItemByGameID(pSearchCorrespond->dwGameID);
			}

			//查找用户
			if (pSearchCorrespond->szNickName[0]!=0)
			{
				_tcslwr(pSearchCorrespond->szNickName);
				ResultUserItem[1]=m_GlobalInfoManager.SearchUserItemByNickName(pSearchCorrespond->szNickName);
			}

			//设置结果
			for (BYTE i=0;i<CountArray(ResultUserItem);i++)
			{
				if (ResultUserItem[i]!=NULL)
				{
					//变量定义
					WORD wServerIndex=0;

					//查找房间
					do
					{
						//查找房间
						CGlobalServerItem * pGlobalServerItem=ResultUserItem[i]->EnumServerItem(wServerIndex++);

						//终止判断
						if (pGlobalServerItem==NULL) break;
						if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

						//索引定义
						WORD wIndex=SearchCorrespond.wUserCount++;

						//辅助信息
						SearchCorrespond.UserRemoteInfo[wIndex].cbGender=ResultUserItem[i]->GetGender();
						SearchCorrespond.UserRemoteInfo[wIndex].cbMemberOrder=ResultUserItem[i]->GetMemberOrder();
						SearchCorrespond.UserRemoteInfo[wIndex].cbMasterOrder=ResultUserItem[i]->GetMasterOrder();

						//用户信息
						SearchCorrespond.UserRemoteInfo[wIndex].dwUserID=ResultUserItem[i]->GetUserID();
						SearchCorrespond.UserRemoteInfo[wIndex].dwGameID=ResultUserItem[i]->GetGameID();
						lstrcpyn(SearchCorrespond.UserRemoteInfo[wIndex].szNickName,ResultUserItem[i]->GetNickName(),LEN_NICKNAME);

						//房间信息
						SearchCorrespond.UserRemoteInfo[wIndex].wKindID=pGlobalServerItem->GetKindID();
						SearchCorrespond.UserRemoteInfo[wIndex].wServerID=pGlobalServerItem->GetServerID();
						lstrcpyn(SearchCorrespond.UserRemoteInfo[wIndex].szGameServer,pGlobalServerItem->m_GameServer.szServerName,LEN_SERVER);

					} while (true);
				}
			}

			//发送数据
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REMOTE_SERVICE,SUB_CS_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}

	return false;
}

//管理服务
bool CAttemperEngineSink::OnTCPNetworkMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SYSTEM_MESSAGE:	//系统消息
		{
			//发送通知
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_SYSTEM_MESSAGE,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_PROPERTY_TRUMPET:  //喇叭消息
		{
			//发送通知
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_PROPERTY_TRUMPET,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_VIP_TRUMPET:
		{
			//发送通知
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_VIP_TRUMPET,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_VIPUpgrade:
		{
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_VIPUpgrade,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_BROAD_LABA:
		{
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_BROAD_LABA,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_SystemBroadcast:
		{
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_SystemBroadcast,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_MatchScore:
		{
			CMD_CS_C_MatchScore * pMatchScore = (CMD_CS_C_MatchScore *)pData;

			WORD wServerID = 0;
			POSITION Position=NULL;
			do
			{
				CGlobalUserItem * pGlobalUserItem = m_GlobalInfoManager.EnumUserItem(Position);
				if (pGlobalUserItem != NULL)
				{
					if (!pGlobalUserItem->IsAndroid() && pMatchScore->dwUserID == pGlobalUserItem->GetUserID())
					{
						if (pGlobalUserItem->m_GlobalServerItemArray.GetCount() > 0)
						{
							wServerID = pGlobalUserItem->m_GlobalServerItemArray[0]->GetServerID();
							break;
						}
					}
				}
			} while (Position != NULL);

			bool bHave = false;
			for (unsigned int i = 0; i < m_vMatchRankInfo.size(); i++)
			{
				tagMatchRankInfo & mri1 = m_vMatchRankInfo[i];
				if (mri1.dwUserID == pMatchScore->dwUserID)
				{
					bHave = true;
					mri1.lMatchScore = pMatchScore->lMatchScore;
					mri1.bActive = true;
					mri1.wServerID = wServerID;
					break;
				}
			}

			if (!bHave)
			{
				tagMatchRankInfo MatchRankInfo = {0};
				MatchRankInfo.dwUserID = pMatchScore->dwUserID;
				MatchRankInfo.lMatchScore = pMatchScore->lMatchScore;
				MatchRankInfo.bActive = true;
				MatchRankInfo.wServerID = wServerID;
				MatchRankInfo.nLastMatchRank = -1;
				m_vMatchRankInfo.push_back(MatchRankInfo);
			}

			return true;
		}
	case SUB_CS_C_MatchScoreBegin:
		{
			return true;
		}
	case SUB_CS_C_MatchScoreFinish:
		{
			std::sort(m_vMatchRankInfo.begin(), m_vMatchRankInfo.end(), compareMatchRank);
			for (unsigned int i = 0; i < m_vMatchRankInfo.size(); i++)
			{
				tagMatchRankInfo & MatchRankInfo = m_vMatchRankInfo[i];
				MatchRankInfo.nLastMatchRank = MatchRankInfo.nMatchRank;
				MatchRankInfo.nMatchRank = i + 1;
			}
			for (unsigned int i = 0; i < m_vMatchRankInfo.size(); i++)
			{
				tagMatchRankInfo & MatchRankInfo = m_vMatchRankInfo[i];

				if (MatchRankInfo.lMatchScore <= 0)
				{
					continue;
				}

				CMD_CS_S_MatchRank mr = {0};
				mr.dwUserID = MatchRankInfo.dwUserID;
				mr.lMatchScore = MatchRankInfo.lMatchScore;
				mr.nCurrentRank = MatchRankInfo.nMatchRank;
				if (MatchRankInfo.nLastMatchRank > 0)
				{
					mr.nChangeRank = MatchRankInfo.nLastMatchRank - MatchRankInfo.nMatchRank;
				}
				else
				{
					mr.nChangeRank = 0;
				}

				if (MatchRankInfo.bActive)
				{
					MatchRankInfo.bActive = false;
					SendDataToGame(MatchRankInfo.wServerID, MDM_CS_MANAGER_SERVICE, SUB_CS_S_MatchRank, &mr, sizeof(mr));
				}
			}

			return true;
		}
	case SUB_CS_C_PEARL_INFO:
		{
			if (wDataSize != sizeof(CMD_CS_C_PearlInfo))
			{
				return true;
			}
			CMD_CS_C_PearlInfo * pPearlInfo = (CMD_CS_C_PearlInfo *)pData;
			lYesterdayScore = pPearlInfo->lYesterdayScore;
			if (lYesterdayScore > 100000000)
			{
				lYesterdayScore = 100000000;
			}
			InitPearlInfo();

			return true;
		}
	case SUB_CS_C_MATCH_SIGNUP:
		{
			if (wDataSize != sizeof(CMD_CS_C_MatchSignUp))
			{
				return true;
			}

			CMD_CS_C_MatchSignUp * pMatchSignUP = (CMD_CS_C_MatchSignUp *)pData;
			if (m_bIsGameBegin&&m_nCurrentMatchID==pMatchSignUP->nMatchID)
			{
				return true;
			}

			DBR_GC_InsertMatchSignUpUser stMatchSignUP = {0};
			stMatchSignUP.dwUserID = pMatchSignUP->dwUserID;
			stMatchSignUP.wServerID = pMatchSignUP->wServerID;
			stMatchSignUP.nMatchID = pMatchSignUP->nMatchID;
			stMatchSignUP.llUserScore = pMatchSignUP->llUserScoreInGame;
			// Game Begin deduct the score on user
			stMatchSignUP.bIsInGame = true;

			if (pMatchSignUP->nMatchID == 0)
			{
				printf("invalid MatchID\n");
				return true;	
			}

			WORD wGroupID = 0;
			for (unsigned int i = 0; i < m_vMatchData[pMatchSignUP->nMatchID - 1].size(); i++)
			{
				if(m_vMatchData[pMatchSignUP->nMatchID - 1][i].size() < m_wUserNumPerGroup)
				{
					wGroupID = i + 1;

					break;
				}
			}

			stMatchSignUP.wGroupID = wGroupID;

			printf("post match signup user\n");
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GC_INSERT_MATCH_SIGNUP_USER,0,&stMatchSignUP,sizeof(DBR_GC_InsertMatchSignUpUser));

			return true;
		}
	case SUB_CS_C_MATCH_USERSCORE_UPDATE:  // GameServer query the update score
		{
			if (m_nCurrentMatchID == 0||!m_bIsGameBegin)
			{
				return true;
			}
		
			CMD_CS_C_MatchUserScore * pMatchUsersScoreForGameServer = (CMD_CS_C_MatchUserScore *) pData;
			WORD wDataSize = 0;
			wDataSize = sizeof(WORD) * 3 + sizeof(tagMatchUserInfo) * pMatchUsersScoreForGameServer->wUserCnt;
			if (wDataSize != pMatchUsersScoreForGameServer->wDataSize)
			{
				printf("wdatasize is %ld, MatchUsersScoreForGameServer->wDataSize = %ld\n", wDataSize, pMatchUsersScoreForGameServer->wDataSize);
				printf("return false match score update\n");
				return true;	
			}

			printf("receive the update score from gameserver\n");
			tagMatchUserInfo * ptmpMatchUserInfo = NULL;
			WORD wTmpGroupID = 0;
			for (int i = 0; i < pMatchUsersScoreForGameServer->wUserCnt; i ++)
			{
				ptmpMatchUserInfo = &(pMatchUsersScoreForGameServer->aMatchUserInfo[i]);
				wTmpGroupID = ptmpMatchUserInfo->wGroupID;
				printf("wTmpGroupID = %ld, MatchDataCurrent TmpGroupSize = %d\n", wTmpGroupID, m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1].size());
				for (int j = 0; j < m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1].size(); j++)	
				{
					if ((ptmpMatchUserInfo->dwUserID == m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].dwUserID)&&(m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].bIsRobot==false))	
					{
						printf("lMatchScore = %lld, ptmpMatchUserInfo->lUserScore = %lld\n", m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].lMatchScore, ptmpMatchUserInfo->lUserScore);
						m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].lMatchScore += ptmpMatchUserInfo->lUserScore;
						continue;	
					}
					else if (m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].bIsRobot)
					{
						int nTime= 10;
						if (m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].wServerID==m_nNewPlayerServerId)
						{
							nTime =10;
						}
						else if (m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].wServerID==m_nHundredServerId)
						{
							nTime =100;
						}
						else if (m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].wServerID==m_nThousandServerId)
						{
							nTime =1000;
						}
						else if (m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].wServerID==m_nTenThousandServerId)
						{
							nTime =10000;
						}
						m_vMatchData[m_nCurrentMatchID - 1][wTmpGroupID-1][j].lMatchScore +=rand()%150*nTime;
					}
				}

				ptmpMatchUserInfo = NULL;
				wTmpGroupID = 0;
			}

			// sort the send list 
			for (int i = 0; i < m_wGroupCnt; i++)
			{
				std::sort (m_vMatchData[m_nCurrentMatchID - 1][i].begin(), m_vMatchData[m_nCurrentMatchID - 1][i].end(),Sort_Match_Score());
				for(int j = 0; j < m_vMatchData[m_nCurrentMatchID - 1][i].size(); j++)
				{
					m_vMatchData[m_nCurrentMatchID - 1][i][j].cbRank = j + 1;
					//printf("%d排名=%d\n",j,m_vMatchData[m_nCurrentMatchID - 1][i][j].cbRank);
				}
			}

			// pack the send data
			CMD_CS_S_MatchScoreUpdate stMatchScoreUpdate;
			stMatchScoreUpdate.wGroupCnt = m_wGroupCnt;
			stMatchScoreUpdate.wUserNumPerGroup = m_wUserNumPerGroup; // temp set value 

			// Get Remain Time
			SYSTEMTIME st;
			GetLocalTime(&st);

			// fill the match info structure
			tagMatchConfigItem stMatchConfigItem = {0};
			GetCurrentMatchInfo(m_nCurrentMatchID, stMatchConfigItem);

			stMatchScoreUpdate.nSecond = ((stMatchConfigItem.nEndTime/100)*3600+stMatchConfigItem.nEndTime%100 * 60)- ((st.wHour)*3600+ st.wMinute * 60 + st.wSecond)+m_nDifferentTime;
			printf("与数据库差异时间=%d,剩余时间=%d\n",m_nDifferentTime,stMatchScoreUpdate.nSecond);
			WORD wMatchUserCnt = 0;
			WORD wCurrentID = 0;
			printf("**************************************\ngroup count is %hu\n", m_wGroupCnt);
			for (int i = 0 ; i < m_wGroupCnt; i++)
			{
				printf("the size of the m_vMatchData[m_nCurrentMatchID - 1][i] is %d\n",  m_vMatchData[m_nCurrentMatchID - 1][i].size());
				for(int j = 0; j < m_vMatchData[m_nCurrentMatchID - 1][i].size(); j++)
				{
					wCurrentID = i * m_wUserNumPerGroup + j;
					stMatchScoreUpdate.aMatchUserUpdInfo[wCurrentID].dwUserID = m_vMatchData[m_nCurrentMatchID - 1][i][j].dwUserID;
					lstrcpyn(stMatchScoreUpdate.aMatchUserUpdInfo[wCurrentID].szNickName,m_vMatchData[m_nCurrentMatchID - 1][i][j].szNickName,CountArray(stMatchScoreUpdate.aMatchUserUpdInfo[wCurrentID].szNickName));

					stMatchScoreUpdate.aMatchUserUpdInfo[wCurrentID].lUserScore = m_vMatchData[m_nCurrentMatchID - 1][i][j].lMatchScore;
					wMatchUserCnt++;

					//printf("Update info send to GS: UserID is %u, lUserScore is %lld\n", stMatchScoreUpdate.aMatchUserUpdInfo[wCurrentID].dwUserID,stMatchScoreUpdate.aMatchUserUpdInfo[wCurrentID].lUserScore);
				}
			}
			lstrcpyn(stMatchScoreUpdate.szMatchTitle,stMatchConfigItem.szMatchName,sizeof(stMatchScoreUpdate.szMatchTitle));

			WORD wSendDataSize = 0;
			wSendDataSize = wMatchUserCnt * sizeof(tagUpdateInfoItem) + sizeof(WORD) * 4 + sizeof(int) + sizeof(stMatchScoreUpdate.szMatchTitle);  //calc the size of the CMD_CS_S_MatchScoreUpdate 
			stMatchScoreUpdate.wGroupCnt = m_wGroupCnt;
			stMatchScoreUpdate.wUserCnt = wMatchUserCnt;
			printf("UserCnt is %u\n", stMatchScoreUpdate.wUserCnt);
			for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID - 1].size(); i++)
			{
				// test
				printf("给房间发送分数改变id=%d\n",m_vMatchGameServerID[m_nCurrentMatchID - 1][i]);
				SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID-1][i],MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_SCORE_UPDATE, &stMatchScoreUpdate, wSendDataSize);
			}

			return true;
		}
	case SUB_CS_C_MATCH_USERINFO_RELOAD:
		{
			if (wDataSize != sizeof(CMD_CS_C_MatchUserInfo_Reload))
			{
				printf("datasize wrong\n");
				return true;
			}

			printf("receive reload request from GS\n");

			CMD_CS_C_MatchUserInfo_Reload * pMatchUserInfoReload = (CMD_CS_C_MatchUserInfo_Reload *)pData;

			if (pMatchUserInfoReload == NULL)
			{
				printf("In para pointer is null\n");	
				return true;
			}

			CMD_CS_C_MatchUserInfo_Reload_Res stMatchUserIfoReload = {0};
			stMatchUserIfoReload.dwUserID = pMatchUserInfoReload->dwUserID;
			stMatchUserIfoReload.wCount = 0;
			stMatchUserIfoReload.wDataSize = 0;

			bool bIsReload = false;

			WORD wInsertCount = 0;
				
			if (m_vMatchData.size() <= 0)
			{
				printf("the size of the match data is zero");
				return true;		
			}

			// 遍历所有信息, 找出用户信息
			for (int i = 0; i < m_vMatchData.size(); i ++)
			{
				if (m_vMatchData[i].size() <= 0)
				{
					continue;
				}

				for (int j = 0; j < m_vMatchData[i].size(); j++)
				{
					if (m_vMatchData[i][j].size() <= 0)
					{
						continue;
					}

					for (int k = 0; k < m_vMatchData[i][j].size(); k ++)
					{
						if (m_vMatchData[i][j][k].dwUserID == pMatchUserInfoReload->dwUserID)
						{
							stMatchUserIfoReload.aMatchUserInfoReload[wInsertCount].dwUserID = m_vMatchData[i][j][k].dwUserID;
							stMatchUserIfoReload.aMatchUserInfoReload[wInsertCount].lUserScore = 0;
							stMatchUserIfoReload.aMatchUserInfoReload[wInsertCount].wGroupID = m_vMatchData[i][j][k].wGroupID;
							stMatchUserIfoReload.aMatchUserInfoReload[wInsertCount].wMatchID = m_vMatchData[i][j][k].cbMatchNum;

							m_vMatchData[i][j][k].wServerID = pMatchUserInfoReload->wServerID;
							wInsertCount ++;
							bIsReload = true;

							// Insert the ServerID with Match
							if(std::find(m_vMatchGameServerID[i].begin(), 
								m_vMatchGameServerID[i].end(), 
								pMatchUserInfoReload->wServerID) == m_vMatchGameServerID[i].end()) 
							{
								printf("find and push back serverid\n");
								m_vMatchGameServerID[i].push_back(pMatchUserInfoReload->wServerID);
							}

							printf("Reload info insert UserID is %u, MatchScore is %lld, GroupID is %hu\n", 
								m_vMatchData[i][j][k].dwUserID,
								m_vMatchData[i][j][k].lMatchScore,
								m_vMatchData[i][j][k].wGroupID);
						}
					}
				}
			
			}

			stMatchUserIfoReload.wCount = wInsertCount;
			stMatchUserIfoReload.bIsGameBegin = m_bIsGameBegin;
			stMatchUserIfoReload.wDataSize = sizeof(CMD_CS_C_MatchUserInfo_Reload_Res)
				- sizeof(tagMatchUserInfoReloadRes) * (100 - stMatchUserIfoReload.wCount);

			printf("S send reload info to GS, ServerID is %u, datasize is %u, insert count is %u\n", 
				pMatchUserInfoReload->wServerID,
				stMatchUserIfoReload.wDataSize,
				stMatchUserIfoReload.wCount);
				

			SendDataToGame(pMatchUserInfoReload->wServerID,MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_USERINFO_RELOAD_RES, &stMatchUserIfoReload, stMatchUserIfoReload.wDataSize);
			return true;
		}
	case SUB_CS_C_QUERY_FAKE_SERVERINFO:
		{
			// global get server count ; package the info
			// CMD_CS_S_FakeServerInfo tagFakeServerInfo
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_MANAGER_SERVICE,SUB_CS_S_QUERY_FAKE_SERVERINFO,&SearchCorrespond,wHeadSize+wItemSize);
		}
	}

	return false;
}

//发送列表
bool CAttemperEngineSink::SendServerListItem(DWORD dwSocketID)
{
	//变量定义
	WORD wPacketSize=0L;
	POSITION Position=NULL;
	BYTE cbBuffer[SOCKET_TCP_PACKET];

	//发送信息
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INFO);

	//收集数据
	do
	{
		//发送数据
		if ((wPacketSize+sizeof(tagGameServer))>sizeof(cbBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,cbBuffer,wPacketSize);
			wPacketSize=0;
		}

		//获取对象
		tagGameServer * pGameServer=(tagGameServer *)(cbBuffer+wPacketSize);
		CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.EnumServerItem(Position);

		//设置数据
		if (pGlobalServerItem!=NULL)
		{
			wPacketSize+=sizeof(tagGameServer);
			CopyMemory(pGameServer,&pGlobalServerItem->m_GameServer,sizeof(tagGameServer));
		}

		printf("%s %s\n", pGameServer->szServerName, pGameServer->szServerAddr);

	} while (Position!=NULL);

	//发送数据
	if (wPacketSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,cbBuffer,wPacketSize);

	//发送完成
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_FINISH);

	return true;
}

//房间发送
bool CAttemperEngineSink::SendDataToGame(WORD wServerID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//查找房间
	CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(wServerID);
	if (pGlobalServerItem==NULL) return false;

	//获取参数
	WORD wBindIndex=pGlobalServerItem->GetIndex();
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//发送数据
	DWORD dwSocketID=pBindParameter->dwSocketID;
	m_pITCPNetworkEngine->SendData(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//用户发送
bool CAttemperEngineSink::SendDataToUser(WORD wServerID, DWORD dwUserID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

void CAttemperEngineSink::GetNextMatchInfo(tagMatchConfigItem &MatchConfigItem)
{
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);
	for (std::list<tagMatchConfigItem>::iterator iter=m_MatchConfigItemList.begin(); iter!=m_MatchConfigItemList.end(); iter++)
	{
		int nMatchStartTime = (iter->nStartTime)/100 * 60 + (iter->nStartTime)%100;
		int nCurrentTime = SystemTime.wHour * 60 + SystemTime.wMinute;
		int nDiffTime = nMatchStartTime - nCurrentTime;
		if (nDiffTime >=m_nMessageRemindTime3 && nDiffTime<=m_nMessageRemindTime1)
		{
			//printf("获得下一场比赛时间=%d,比赛类型=%d\n",iter->nStartTime,iter->nMatchType);
			MatchConfigItem.nApplyCost = iter->nApplyCost;
			MatchConfigItem.nEndTime = iter->nEndTime;
			MatchConfigItem.nGameTime = iter->nGameTime;
			MatchConfigItem.nMatchNum = iter->nMatchNum;
			MatchConfigItem.nMatchType = iter->nMatchType;
			MatchConfigItem.nMatchUserCount = iter->nMatchUserCount;
			MatchConfigItem.nStartTime = iter->nStartTime;
			lstrcpyn(MatchConfigItem.szMatchName,iter->szMatchName,sizeof(MatchConfigItem.szMatchName));
			return;
		}
	}
	return;
}

//发送通知信息 给所有的比赛玩家发送统一的消息, 这个统一的消息的结构大小由wAttachDataSize决定
bool CAttemperEngineSink::SendMatchNotifyInfoBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wAttachDataSize)
{
	if (m_nCurrentMatchID == 0)
	{
		return false;
	}

	// initialization the send structure
	CMD_CS_S_MatchInfoNotify stMatchInfoNotify;
	stMatchInfoNotify.vUserIDInGameRoom.clear();
	stMatchInfoNotify.pData = new byte[wAttachDataSize];
	memcpy(stMatchInfoNotify.pData, pData, wAttachDataSize);

	// Send data size
	WORD wSendDataSize = 0;
	for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID - 1].size(); i++)
	{
		for (int j = 0; j < m_vMatchData[m_nCurrentMatchID - 1].size(); j ++)	
		{
			for (int k = 0; k < m_vMatchData[m_nCurrentMatchID - 1][j].size(); k ++)	
			{
				if(m_vMatchGameServerID[m_nCurrentMatchID - 1][i] == m_vMatchData[m_nCurrentMatchID - 1][j][k].wServerID)
				{
					stMatchInfoNotify.vUserIDInGameRoom.push_back(m_vMatchData[m_nCurrentMatchID - 1][j][k].dwUserID);	
				}
			}
		}

		stMatchInfoNotify.wVectorSize = stMatchInfoNotify.vUserIDInGameRoom.size();
		stMatchInfoNotify.wDataSize = wAttachDataSize;
		wSendDataSize = wAttachDataSize + stMatchInfoNotify.vUserIDInGameRoom.size() * sizeof(DWORD);
		SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID - 1][i], wMainCmdID, wSubCmdID, &stMatchInfoNotify, wSendDataSize);

		wSendDataSize = 0;
		stMatchInfoNotify.vUserIDInGameRoom.clear();
	}

	// free the mem
	delete[] stMatchInfoNotify.pData;

	return true;
}

//给每个GameServer发送对应的User的信息
bool CAttemperEngineSink::SendMatchUserInfoBatch(WORD wMainCmdID, WORD wSubCmdID)
{
	if (m_nCurrentMatchID == 0)
	{
		printf ("Invalid Current Match ID\n");
		return false;
	}

	WORD wSendDataSize = 0;
	CMD_CS_S_UserInfo stUserInfo;
	stUserInfo.vUserInfo.clear();
	for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID - 1].size(); i++)
	{
		for (int j = 0; j < m_vMatchData.size(); j ++)	
		{
			for (int k = 0; k < m_vMatchData[j].size(); k ++)	
			{
				if(m_vMatchGameServerID[m_nCurrentMatchID - 1][i] == m_vMatchData[m_nCurrentMatchID - 1][j][k].wServerID)
				{
					stUserInfo.vUserInfo.push_back(m_vMatchData[m_nCurrentMatchID - 1][j][k]);	
				}
			}
		}

		stUserInfo.wVectorSize = stUserInfo.vUserInfo.size();
		wSendDataSize = sizeof(WORD) + stUserInfo.vUserInfo.size() * sizeof(tagMatchItem);
		SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID - 1][i], wMainCmdID, wSubCmdID, &stUserInfo, wSendDataSize);

		wSendDataSize = 0;
		stUserInfo.vUserInfo.clear();
	}

	return true;
}

void CAttemperEngineSink::GetCurrentMatchInfo(int nMatchNum, tagMatchConfigItem & MatchConfigItem )
{
	for (std::list<tagMatchConfigItem>::iterator iter=m_MatchConfigItemList.begin(); iter!=m_MatchConfigItemList.end(); iter++)
	{
		if (nMatchNum==iter->nMatchNum)
		{
			MatchConfigItem.nApplyCost = iter->nApplyCost;
			MatchConfigItem.nEndTime = iter->nEndTime;
			MatchConfigItem.nGameTime = iter->nGameTime;
			MatchConfigItem.nMatchNum = iter->nMatchNum;
			MatchConfigItem.nMatchType = iter->nMatchType;
			MatchConfigItem.nMatchUserCount = iter->nMatchUserCount;
			MatchConfigItem.nStartTime = iter->nStartTime;
			lstrcpyn(MatchConfigItem.szMatchName,iter->szMatchName,sizeof(MatchConfigItem.szMatchName));
		}
	}
	return;
}

//发送比赛结束消息给服务器
bool CAttemperEngineSink::SendMatchEndInfoToServer(int nMatchType)
{
	if (m_nCurrentMatchID == 0)
	{
		return false;
	}
	WORD wSendDataSize = 0;
	CMD_CS_S_MatchEndInfo stUserInfo;
	stUserInfo.vUserMatchEndInfo;
	memset(stUserInfo.vUserMatchEndInfo,0,sizeof(stUserInfo.vUserMatchEndInfo));
	stUserInfo.wMatchType=nMatchType;
	stUserInfo.wVectorSize=0;
	for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID - 1].size(); i++)
	{
		for (int j = 0; j < m_vMatchData[m_nCurrentMatchID-1].size(); j ++)	
		{
			for (int k = 0; k < m_vMatchData[m_nCurrentMatchID - 1][j].size(); k ++)	
			{
				if(m_vMatchGameServerID[m_nCurrentMatchID - 1][i] == m_vMatchData[m_nCurrentMatchID - 1][j][k].wServerID)
				{
					stUserInfo.vUserMatchEndInfo[stUserInfo.wVectorSize].dwUserID = m_vMatchData[m_nCurrentMatchID - 1][j][k].dwUserID;
					stUserInfo.vUserMatchEndInfo[stUserInfo.wVectorSize].nRanking = m_vMatchData[m_nCurrentMatchID - 1][j][k].cbRank;
					stUserInfo.vUserMatchEndInfo[stUserInfo.wVectorSize].lUserScore = m_vMatchData[m_nCurrentMatchID - 1][j][k].lMatchScore;
					//添加奖励配置信息
					stUserInfo.wVectorSize++;
				}
			}
		}
		wSendDataSize = sizeof(WORD)*2 + stUserInfo.wVectorSize * sizeof(tagMatchEndInfo);
		printf("发送比赛结果数据wSendDataSize=%d, ServerID is %d\n",wSendDataSize, m_vMatchGameServerID[m_nCurrentMatchID - 1][i]);
		SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID - 1][i], MDM_CS_MANAGER_SERVICE, SUB_CS_S_MATCH_END, &stUserInfo, wSendDataSize);

		wSendDataSize = 0;
		stUserInfo.wVectorSize=0;
		memset(stUserInfo.vUserMatchEndInfo,0,sizeof(stUserInfo.vUserMatchEndInfo));
	}
	return true;
}

time_t CAttemperEngineSink::StringToDatetime(const char *strTime)  
{  
	if (NULL == strTime)  
	{  
		return 0;  
	}  
	tm tm_;  
	int year, month, day, hour, minute,second;  
	sscanf(strTime,"%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);  
	tm_.tm_year  = year-1900;  
	tm_.tm_mon   = month-1;  
	tm_.tm_mday  = day;  
	tm_.tm_hour  = hour;  
	tm_.tm_min   = minute;  
	tm_.tm_sec   = second;  
	tm_.tm_isdst = 0;  

	time_t t_ = mktime(&tm_); //已经减了8个时区  
	
	return t_; //秒时间  
}

void CAttemperEngineSink::OnRecord(CString strMsg)
{
	CString strFileDlgPath;
	TCHAR szModuleDirectory[MAX_PATH];	//模块目录
	GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
	int nModuleLen=lstrlen(szModuleDirectory);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
	if (nModuleLen<=nProcessLen) 
		return ;
	szModuleDirectory[nModuleLen-nProcessLen]=0;
	strFileDlgPath = szModuleDirectory;
	CString strThreadLogDir  = strFileDlgPath+"\\Record.log";
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	char szTime[100];
	sprintf( szTime, "日期::%d:%d:%d:%d\r\n",sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
	CString strTime(szTime);
	strMsg = strMsg + strTime;
	CFile fLog;
	if(fLog.Open( strThreadLogDir, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite ))
	{

		fLog.SeekToEnd(); 	
		int strLength=strMsg.GetLength();
#ifdef _UNICODE
		BYTE bom[2] = {0xff, 0xfe};
		fLog.Write(bom,sizeof(BYTE)*2);
		strLength*=2;
#endif
		fLog.Write((LPCTSTR)strMsg,strLength);
		fLog.Close();
	}
}
//////////////////////////////////////////////////////////////////////////////////
