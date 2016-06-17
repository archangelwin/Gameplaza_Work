#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

char *  g_pszFakeName[] = {"��ս�پ�","�������","��˧��������","��������","����������","Ц����˪","�����¸ɱ�","����֮��","���д���","���˾;���","��ʧ���˺�","а������ǹ","�ڶ��ڰ��","������ɽ�ɻ�","��ȻҰ��˧��","һ��ͺ�˧","���ڲ�����","����ι����","���ɸ�","�ܰ��벻�ȶ�һ��","ushdhdebgd","bvddfghhjj","augvss","dghjjhg","142588","Hggtvh","��һ��","���ʲ�","hjij66663","dfghhbbbn","cvgrdd","vuhghjj","0459zd","����","������Ц��ֹ","yvdexm","suggshsh","ydrrdrrr","bxbdveb","okjkohk","jpz011","jpz009","�ܰ�����","6647568","duliang","Hsshshsjs","zg747606","Csydhsshshs","��������","ghjeeff","����999","369077","nnjxjje","�������ɿ���","�������","ˮ���߲ʺ�","sdjp18","�ʽ�С����","9467852","Ywijhmh","���ʮ��","hdghxvgdv","dududu","dtghhjjn","Zhengjing6","���鲶��","Ԥ�深���ƽ�","shouldhave","��·��ģ��","��","ffuvuc","nimala","֪�㳣��","С��8287336","�����ޱ�","ai5215","wqasewa","lw7777lw","Qsadsrff","ѧ��������","��������","������ӳ�","����֪����","�����İ�","ǳ��΢ױ","������Ϣ","d���Ļ���","��ϲ����","�����ǵĴ�","����Ҵ���","���ڵ���ؼBSW","��β����","92������","y1����Ҳ�ܻ�","������΢Ц","����û�����","���������","������","��ʽ��ؼ�µ�","_�����Ѳ�����","����","����","����С����","�両����","adsferf","�ж�����ǿ","��������","ʦ̫�����","ǳ���Ż���","�뵺�Ƶ�","���˸�HelloKitty","����į�ͺ�","��������","���������","����̫ӵ��","Ľ����ʺ��","�����˵İְ�","׷���Ҹ���","�����貽","޹�²ݵ�����","˵���һ�Ц","���౦��","�ո���û�е�","ɽ���еļ�Ʒ","��ʹ�ҵ���","����ס�Ķ���","����������","�ݺ�ƽ��","˹�˶���","���ʹҴ�","ϧЦ������","����������","���ݲп�","īС��","���˾͹���","īī","��¥������","�������","һ��ͺ�˧","����176","��������","ĺɫ΢��","ָ����ӣ��^_^","�ٸ�����","��������","����B����","����һö","��ˮ�̺�","��Ҫ�Լ���","�����ü","������","��С��","Ǿޱ����","���ľͺ�","�������","���ڶ���","���ľͺ�","���ڶ���","��Цؿ����","��������̧��","�����İ�����","[ѧ��������]","����լ��","��Ҫ��ô��ͣ","���ľͺ�","��������","ӵ����","�����Ӱ","���³����","���ٲ���سɻ�","�������һƬ��","����������������","�㲻������","����������","��굽������","����ؼ����ؼ��˧","���ڱ�������ս","�û���꼾","�͵��⸻˧","÷��0012","���������","û��������ק","����װ�ƴ��ҷ�","������ѧ��","װ�Ʋ��ʺ���","��Ǯ��������","��������","���ǹ�","����С��","����������","��ͼ����Լ�","����������","��Ӯ��ͣ","ȫ��˧�ȴ���","���ʲ�ζ��","������������","��̫����","����������","Ը��һ����","�������α","������888","��ү��","С����ɭ","�Ұ�������","����ҹ̫��","����ؼĪ����","��ң����","�����ײ���","����ʹ��","��ս����","Ϊ�Լ�����","���ı��˽���","椸����̳���","�ջ�����","�����������","ҹ����","ͯ�����Ǽ�","��Ȳ���������","��Լ����","�˹���;","��������","��ί���һ����","��������","��ֹ�ŵ�","�ݰݰ���","����Ϊֹ","����ϰ����Ȼ","һ�׼򵥵ĸ�","��ռŮ����","��Ȼ����","��ˮ�޺�","����Ժ����","�͵���Ĭ��","��������","��ˮ�е�ѩ","����ʽ����","�ݺݰ�","���������","С��","�������","����������","���˼��껪","������","һ���į�Ļ�","˼����һ�ֲ�","�Ĳ�����ʹ","90��ح˧��","��������ʹ","ҹ��������","��ʵ̫��ʵ","����˱���","��㯵���;","ע��һ����","�黰��ʦ","Ц������","����Ե��","��˿ŮHumane","������΢Ц","����˭����˵","�Ȳ�������","�ü�įԶ��","��������","_ǳ��ɫDe��","ˮ���Ƶ�","����Ҹ�","��������ϥ","̤ѩѰ÷","�ý���","��Ӱ���","�к�����","�쾵����","������¥","���߲�����","�����ޱ�","��ˮ��Ƽ","��������","������","�����޺���","������ˮ","���販","����׷�Ұ�����","������","����ޱ��","�������","�غƴ�","������Ǽ������","��į����","ҳ��","����ʫ��","ҹ�Ĳ�ƽ��","�ź����Ƕ���","����Ѷ�","���������","�������","Ľ������","����ʽ��ˮ","����Ҳ�᲻��ǿ","�����˾�Сɧ��","С���˴��ǻ�","������������","��ô����ôԶ","�������˸���","�þӹµ�������","�Ӳ��͵�","���Ӵ��","΢΢һЦ��쳾","��Щ��������","������Ĩ��","С��ֻ��������","��˹��","�����","��������","���ڼ�į","����C��","��ή��ڻ�","���겻��ҵ","��������","͵�Ĵ��","����","�ٻ�����İ·","β��İ�","����ʿ��ȱ��","�컯Ū��","��С��","δ�����","������ϲ��","������ҵ������","�����ҵ�����","�Բ�����޴���","����������","100������","������˾��ǰ�","��Ӿ����","�ƽ�����","˧������¥","����de����","������99","��ţû��ţ","���ڻ���","Royal����è","С����","���������","��ѽ��ѽӴ","����ţ�����","��Ӯ�Ż�ƴ","ת�ð������","����wifi������","����޿�����","Ц��������","��������","���������ֺǺ�","�ռ���ǿ","����վֱ��ſ��","��������ν��","��Ϊ�Լ�����","�Ƽ�ɵ�","�ഺ���԰�","��4ק","˵��̫����","����Э��᳤i","����������","�ݰ˺Ⱦ�˵ʮ��","�����Ҹ�Ǯ��","����Ҳ����","������","÷������","�����İ�","�¶�","��ըƤ����","��βů���׳���","ת��˵�ټ�","�ҹ�����������","��ȱ������","װ�ƽ翸����","һ��С����","����Ц�쳾","��������ӭ��","ʫ�Ƴ�����","�Ѿ��ٷ�","������","ugugugw","��ݼ����","njf566","���ƴ���","jedndbj","liangshanbo","tugugug","hsjsjhsj","gjefduf","3ed4dx","wc80077","hvcdsf2fhjjj","eudjejm","dounimei","����Ϯһ��","gbthghgihhh","sdjp16","ddtyyuff","junjun520","dhxydgennx","��������","vyvyvubvb","��������","udndjdjfidj","�¹��ȷ������","����","����13","dhfjshs","С������","123456ĭĭ","hduderh","HRsitser","jxx41115","ytfghrd","nishikou44","sgxxhh","qtjmxj","hdhd222","hcijfnkddd","eedddsjjejsee","qianerdenger","wwhuhddhh","�Խ���ͷ","xfghrdcjkkm","liuyangming8","zo747nii","uddjjd2222","������Ȼ","��ī����Ŷ","��÷��������","��^^��","����è","��ӭɧ��","һ��±��_","ι˵����","���ֹ���û��","��С��^^","��Ȼ������","�ϵ��Ǹ��","��ĸ�����","���Ƕ�����˭","������β��","��ˮ�����˶�","����ɽ��ƨؼ","���̿���","ʺ������","32����Ȣ��","�ñ��ϲ���","��������ϧ","��ĸ������","���ӿ�","��������","����ָ����","��������","��һ�����̻�","�����ȷ��","�����߲�","�䰮","����߲���","�ĵ�δ��","����·�˼�","��ʵ�Ҳ�ɵ","huxian888","xsssssj","zl1382","���˰���Ӯ","¬��ô123","����ж��","iuy9688","abcdbb","wz199611","ҹ������","dq2563","ssseezzz","zo8787","jzk130708","tu908553","gyffd3555","xgfkfhg","hnhfguuu7","mnbhjkl123","bnnhgtffff","123djj","huxian777","accdaaaa","�Ͻ���","����������","wwwewweq","dq13468","123djm","023123vvkfjrt","x814420053","zzzjjj88","Gujnnj","kkjnxkbbji","uvf67i8","�����123","Chu����̫����","������ʵ1","rgijgg","jsnsnsnxs","�����Ǹ�ү����","��������","rddgg448","��������","vbhgyunjik","jjdjdde","fghjkkhj","bhdhvsge","hdbxbdjdn","vgy123580","nimeim","������123","Zhangge","e888888e","kkkeerp","ffgtgtgw","nsnsnsnd","���˶���yyy","hgfu8765","û������i","��ս������","123z4g5m","r561826574","zhaihao","cccxxxz","fxfgg1478","nsnwwww","wc88imdii","jxhgdgd","xcv11111","ygffvh","jw77178ej","aiaiaiai","tuvf668","bdu6wh8i","��õ�","jjjjnjnnh","ftygfhuh","ycyfhchvj","jspeibipo1","hbdbjdhsh","��������464","gddyjh","����ը��","yangyan","fig8f8f8","�Ʒ�38","4ch6bj","eraaatj","gsggshhsh","�ҾͲ�������","jssnsnseee","33441122","srffgvhhj","������ĥ��","��������","zsyzjl123","����Ҳ","ghjcgcg","xbhdhdjzj","rgyihf","����008","dtfhvhh","4r5668","jkajtdnw","rfguiiooll","cyydgj","�㿩logo","˵����ָ�","����С����","��¥��������","6869583","12rsftgg","nsnsnsss","���0317","2271343231","��̲�ȼ�","fdsuhggji","����ʹ","�����ල�ֵ�","�������»�","xiaowan","����8","dingggnajhg","jhffddhhj","bdhchhfyd","С��ֽ��J","gucgju","d666666d","߼��","�÷����ü���","jyfddt","�ް���","С������","gugugyyj","Ŭ���˾����˰�","gghhjnv","Dingsidang","��Ŷ��һ��","w24j12x11","gdjjjrbfd","dfvyyhj","���Ը߸����","jdhgejdvd","teghehdh","787yuyy7t6y7","Xnxidsn","15515515wu","a895558","pjantgan","jsjsjssjshsh","tyu677","jhdhdhfn","12315q","��������˵11","cm����Ѫpo","xvxgdgd","�ݼ���","dmwgua","vy316419js","���","ttd3467","��Ů��ɫ","king��˴���","����ǧ����","dhhvdgh","������12323","jdhfhdjhdve","�Ŀ���ıni","����","fyhbhh","dssryhg","lkkijo","�����´���","lovemm","ftgmjg","dgdgueuege","�л���","��Ҿ��÷·��","qingang1","ysgeywgjsh","d12332","a8957480","gfh654","vjghcf","jdhdbbdbddbn","wzh530","qwevbjk123","��������","bae111����","huzijjdajjda","Xiao��","dmtptdt","Bxjsne","Ҳ���Ц�Ǻ�","hcheue369","�Ծ�����","xl461126464","gdjhdtuf","yiyuhen","asz898","�㽭����","w7777lw","w6666lw","xtrxtrysss","ss1314520","jjuuiju","ͬ��ɽ��","��ܽ��","yyyyyyying","etwyug","ֻ������","������15745","fhfbfcbdnd","548064","������a","��aaaaaa","gggfdee","111111w","wyx333","jxl1314","12377y","����DJ","jhcrty","rrttyyuuii","����998","vcdtgb","����666","��־","dtugdrj","ghssef","�����","nhfdry","n00544","hbvfdewsxz","hsgech","ghggggb","jsnsmddd","hgtyhg6","�ڶ������","���Ų�","11111f","seezddeewe","dongzi5","�ҷ�����","�ո��޵а汾","a15fgcfff","�仰��","hhhyivkv","gghgggh","���","yhgghl","qwsddff","dongzi4","252525nn","yyegggdggh","frgji5536","a2863115082","usjejesww","tszxhs","w6719625","zxcvbnmlinb","wrjfiydi","a65001234","�����11","�������110","a15070041","dongzi3","fmfjhhjyfj","������Ҳ","ר�Ҵ���","uoa5866999","���������","������","jhjhuuhjj","778899oo","aaz1234","������","djjjjjkk","hahaasre","������","u��v����","���ܷǹٷ�","puj2888856","Hdjfjfjfjf","139968146","agily16","777777f","xx09616","yesyes777","sssszxd","a15070041","hai1234567","ghjhvbn","q6445320","hehe2520m","dongzi1","lililiuliu","�ҾͲ���֪11","��������","hhhhh222222","����LadyGaga","FZMCZD","tchvds","�ż������","59383566kk","mo1363825","lukais62868","������������","490677","С�����","�µ�Ӣ��","�ȲȲȲ�c","dfghhjjv","���������","juslljz","�ǵò鿴����","xxxdfrs","wzh158","tyffggtdsfg","agily15","djdkww","�۷�ؼ����","wangyui96","huij6366","a15070041kf","jhdhfmf","jdshh34","yuan137094","�ܹؼ�����","huozhi","������Ļ�","ghgffgniu6fg","ngfmjuggr","ghjjgvh","����","�����￩��","dghffd","a650se","cdtdff","520336","�����о�","666tsfwftstw","������ϻ�ba","kkriben","cvhhhyhh","hhcdryjj","������","�½���8","weqasdewa","ddfhvffg","xdfvvcf","Hejhzhdd","555555f","julijuzu","123456wasn","agily14","huzijjdajj","axuda6500","��Ϣ��Ϣxx","jdhhhrhj","888888x","�������","����","�Ҿ�ȥ����11","ajdjjdjd12","����","ijuyttti","ˮ������","mdkjdhffj","�����","610289","fgt123","�������˵�","zk747606","jlh814","�����jiqiren","������","xuziyuan666","���������ֻ�","С��123","�ᵶ����ЦΪ","opl87072","987��","liuyang660","243320242","yyysss14","gplwun","jqx123","guigui12","woshishuiq","hzhhbshsh","529077","binbinbin","wccc8207","�ȿں���һ��","hdbfhgfg","shijie789","890890a9","wywy51","mg528885","uhggff","dkeem433","fresrew","ˮ�����","fhjjfg","snbxhxjzjjsks","С�����е���","Ehejhsn","yoyo52000","�Ʒ�֪��X","wcc88207","hgffhjf","ά����D","Сΰ����","A497396","jpgmwjat","7845124","6tuyffhjj","fghjcfhvcgb","jt12356","�Ը�kk","fhfufhfy","jdbh678","qylpcb","ghjkkkkggjk","hjtpgaujg","woshini","ɶʱ���Ǹ�ͷ","xfso222","kk284628","iiii646464","zu74760","m96n649","wc82078","bhjycvg","������ͼ����","qtmkwn","SisterHeRui","xgfhff","xk19890526","xbjdbjdjj","���ǲ�����","mwtppj","������","y52669","sdjp169","ak47ak","�������","����˺�","ye12356","lilipen","s4gf123","��Ӫÿƽ��","cbgdjc","�����ڴ�","1987118al","yu����yu","343591037b","dbbxdee733","�����","����","�����Բ���","iijoll","cy2234","�����Ľ���i","Ѧ�ճ�","ˮ������","1q1q2w3e","ι��һ��","bb142536","tgtddhr","grhjuhty","23748392","gdfigc","jyujenwt","yangjjj111","a15070041","3wangli","���ɺÿ���","��ҵİ취","lzc8889","456712aq","wmwmpk","ˮ����","jiagang002","������7","Hwؼ����","dd14253","A15132","wanglli2","dcjkfesxvh","QsDsddsf","RMB8000","bhft5678","Ҫni�����","wang7409","mwpvajt","Yufyvyyvu","����","buyummm","a1507004174","bffhihy","���ϵ��ܷ�","260766","������ɱ��","kughjyg","�����ָ�","namjdjekdmd","�Ұ���1314","��ʮ����","���ܷ��㲻","ˮ���","hdgdgr","ly510125","a91271","978568","zjsnjzjz","rir9ed","����Ĺ淶","��������","556680009","���Ľ���","sdfg168","890223nijuan","qwsjjs","����007","a15070804","eeieokfd","���ϵ���","a2010222","d�ȴ�����ô��","Kl7789jsj","xxdfghj","q866052","z7203313","1wangli","hua��2222","long66635","ghjjjjjjjf","�󽭴�","345678ii","ghzjs556889","cc806500","jhhgbnjjj","������","�����3","yffujg","ly520061","qa866052098","ygfgj999","aa0708043","awangli","ˮ��һ","Fe0909","ppplllaa","С����","dmtpwjt","��������","ujjhgcvh","sfvezfd","Ŷ����Ŷ¥Ŷ","jfkfkfmff","poiioooo","��һ�ݻ��ۻ�","ptmwmahx","�ڴ������","ydhh123","shgdyew"};
//��ʱ��
#define IDI_LOTTERY_INFO 10
#define IDI_PEARL 12
#define IDI_TIMER_CHECK_INTERVAL	13			//����ʱ����
#define IDI_READ_MATCH_CONFIG		14			//��ȡ��������
#define IDI_MATCH_BEGIN_REMINDER		15			//������ʼ����ʱ
#define IDI_MATCH_BEGIN		16			//������ʼ
#define IDI_MATCH_END_REMINDER		17			//������������ʱ
#define IDI_MATCH_END		18			//��������
#define IDI_TIMER_LOAD_MATCH_CONFIG	19			//�������ö�ȡ
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

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_wCollectItem=INVALID_WORD;

	//���ñ���
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
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

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//���ɻ�������
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

		//�ж��Ƿ��Ѵ���
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

//��ʼ����������
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

	//��������
	{
		tagRandData rd = {0};
		//�������ַ���3055��
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

		//���ڰ��ڷ���3112��
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

		//����ǧ�ڷ���1334��
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

		//�������ڷ���354��
		rd.wServerID = 317;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 41; j++)//100~500��12300��(40*41/2)*10+100*41��
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

	//��������
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

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
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

	//��ȡ����
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

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_wCollectItem=INVALID_WORD;
	m_WaitCollectItemArray.RemoveAll();

	//���ñ���
	m_pITimerEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�������
	m_GlobalInfoManager.ResetData();

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//ʱ���¼�
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

				//��������ʱ��
				if (LotteryInfo.nSendSecond <= nCurrentSecond)
				{
					nSelectIndex = i;
					//printf("����ʱ�� %d\n", nCurrentSecond);
					break;
				}
			}

			//�н��ɷ�
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
			//��ȡ����
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR szConfigFileName[MAX_PATH] = TEXT("");
			GetCurrentDirectory(sizeof(szPath),szPath);
			_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\GameMatchConfig.ini"),szPath);

			//��ȡ����
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
				//���ر�������
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_MATCH_CONFIG,0,NULL,0L);
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_GET_SQL_TIME,0,NULL,0);
			}
			tagMatchConfigItem sMatchConfigItem={0};
			GetNextMatchInfo(sMatchConfigItem);
			int nMatchStartTimeHour = sMatchConfigItem.nStartTime/100;
			int nMatchStartTimeMinute = sMatchConfigItem.nStartTime%100;

			int nMatchStartTime = 60 * ((sMatchConfigItem.nStartTime)/100 * 60 + (sMatchConfigItem.nStartTime)%100 ); //����Ϊ��λ
			int nCurrentTime = 60*(st.wHour * 60 + st.wMinute)+ st.wSecond - m_nDifferentTime;
			int nDiffTime = nMatchStartTime - nCurrentTime;
			MatchNotification stMatchNotification = {0};
			stMatchNotification.nMatchID = sMatchConfigItem.nMatchNum;
			stMatchNotification.nMatchPeopleNum = sMatchConfigItem.nMatchUserCount;
			stMatchNotification.nMatchScore = sMatchConfigItem.nApplyCost;
			stMatchNotification.nMatchType = sMatchConfigItem.nMatchType;
			/*printf("���ʱ��=%d��RemindTime1=%d��RemindTime2=%d��RemindTime3=%d���Ƿ����ѹ�1=%d,2=%d,3=%d\n",nDiffTime,m_nMessageRemindTime1*60,m_nMessageRemindTime2*60,m_nMessageRemindTime3*60,m_IsRemind[0],m_IsRemind[1],m_IsRemind[2]);*/
			if (nDiffTime>0&&nDiffTime<=(m_nMessageRemindTime1*60)&&m_IsRemind[0]==0)
			{
				m_nCurrentMatchID = sMatchConfigItem.nMatchNum;
				printf("��ǰ��Ϸ����=%d\n",m_nCurrentMatchID);
				//������ʾ��Ϣ
				printf("5���Ӽ��\n");
				m_IsRemind[0]=1;
				_sntprintf(stMatchNotification.szNotification,CountArray(stMatchNotification.szNotification),TEXT("%s����%d���ӿ�ʼ"),sMatchConfigItem.szMatchName,m_nMessageRemindTime1);
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION,&stMatchNotification,sizeof(stMatchNotification),0L);
			}
			else if (nDiffTime>0&&nDiffTime<=(m_nMessageRemindTime2*60)&&m_IsRemind[1]==0)
			{
				m_nCurrentMatchID = sMatchConfigItem.nMatchNum;
				printf("��ǰ��Ϸ����=%d\n",m_nCurrentMatchID);
				//������ʾ��Ϣ
				printf("3���Ӽ��\n");
				m_IsRemind[1]=1;
				_sntprintf(stMatchNotification.szNotification,CountArray(stMatchNotification.szNotification),TEXT("%s����%d���ӿ�ʼ"),sMatchConfigItem.szMatchName,m_nMessageRemindTime2);
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION,&stMatchNotification,sizeof(stMatchNotification),0L);
			}
			else if (nDiffTime>0&&nDiffTime<=(m_nMessageRemindTime3*60)&&m_IsRemind[2]==0)
			{
				m_nCurrentMatchID = sMatchConfigItem.nMatchNum;
				printf("��ǰ��Ϸ����=%d\n",m_nCurrentMatchID);
				//������ʾ��Ϣ
				printf("1���Ӽ��\n");
				memset(m_IsRemind,0,sizeof(m_IsRemind));
				_sntprintf(stMatchNotification.szNotification,CountArray(stMatchNotification.szNotification),TEXT("%s����%d���ӿ�ʼ"),sMatchConfigItem.szMatchName,m_nMessageRemindTime3);
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION,&stMatchNotification,sizeof(stMatchNotification),0L);

				m_pITimerEngine->SetTimer(IDI_MATCH_BEGIN_REMINDER,55000,1,NULL);
				m_pITimerEngine->KillTimer(IDI_TIMER_CHECK_INTERVAL);
			}

		}
		break;
	case IDI_MATCH_BEGIN_REMINDER:
		{
				//������ʾ��Ϣ
				CMD_GPO_MatchTimeNotify stMatchNotification={0};
				stMatchNotification.cbMatchStatus = 0;
				stMatchNotification.nSecond = 5;
				printf("����5�뵹��ʱ��Ϣ��������=%d\n",m_vMatchGameServerID.size());
				tagMatchConfigItem stMatchConfigItem={0};
				GetCurrentMatchInfo(m_nCurrentMatchID,stMatchConfigItem);
				char szPrize[128];
				sprintf(szPrize,"Ӯ%s������ʼ",stMatchConfigItem.szMatchName);
				lstrcpyn(stMatchNotification.szTitle,szPrize,sizeof(stMatchNotification.szTitle));

				for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID-1].size(); i++)
				{
					printf("��Ϸ����id=%d\n",m_vMatchGameServerID[m_nCurrentMatchID-1][i]);
					SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID-1][i],MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_NOTIFICATION_COUNTDOWN,&stMatchNotification, sizeof(CMD_GPO_MatchTimeNotify));
				}
			m_pITimerEngine->SetTimer(IDI_MATCH_BEGIN,5100,1,NULL);
		}
		break;
	case IDI_MATCH_BEGIN:
			{
				//������ʾ��Ϣ
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
				printf("����������Ŀ=%d\n",m_vMatchGameServerID[m_nCurrentMatchID - 1].size());
				for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID - 1].size(); i++)
				{
					printf("����id=%d\n",m_vMatchGameServerID[m_nCurrentMatchID-1][i]);
					SendDataToGame(m_vMatchGameServerID[m_nCurrentMatchID-1][i],MDM_CS_MANAGER_SERVICE,SUB_CS_S_MATCH_BEGIN,&stMatchBegin, sizeof(CMD_CS_S_MatchBegin));
				}
				int nMatchTime = ((stMatchConfigItem.nEndTime/100)*60+stMatchConfigItem.nEndTime%100)-((stMatchConfigItem.nStartTime/100)*60+stMatchConfigItem.nStartTime%100);
				m_pITimerEngine->SetTimer(IDI_MATCH_END_REMINDER,nMatchTime*60000-5000,1,NULL);
				time_t now = time(NULL);
				strftime(m_szMatchBeginTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
				//��Ӽ�����,�Ȼ�����һ�鲻���Ķ���
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
			sprintf(szPrize,"Ӯ%s��������",stMatchConfigItem.szMatchName);
			lstrcpyn(stMatchNotification.szTitle,szPrize,sizeof(stMatchNotification.szTitle));
			for (int i = 0; i < m_vMatchGameServerID[m_nCurrentMatchID-1].size(); i++)
			{
				printf("��Ϸ����id=%d\n",m_vMatchGameServerID[m_nCurrentMatchID-1][i]);
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
					printf("%d����=%d\n",j,m_vMatchData[m_nCurrentMatchID - 1][i][j].cbRank);
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
						pMatchResultItem->nIsGet = 2;//2Ϊ�޽���
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
			printf("���ͱ���������Ϣ����������=%d",stMatchConfigItem.nMatchType);
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
			//���ر���ʱ��������Ϣ
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

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_REGISTER:		//����ע��
		{
			return OnTCPNetworkMainRegister(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_SERVICE_INFO:	//������Ϣ
		{
			return OnTCPNetworkMainServiceInfo(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_USER_COLLECT:	//�û�����
		{
			return OnTCPNetworkMainUserCollect(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
		{
			return OnTCPNetworkMainRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_MANAGER_SERVICE: //�������
		{
			return OnTCPNetworkMainManagerService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//��ȡ��Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//��Ϸ����
	if (pBindParameter->ServiceKind==ServiceKind_Game)
	{
		//��������
		WORD wBindIndex=LOWORD(dwSocketID);

		//���ܴ���
		if (wBindIndex==m_wCollectItem)
		{
			//���ñ���
			m_wCollectItem=INVALID_WORD;

			//�����л�
			if (m_WaitCollectItemArray.GetCount()>0)
			{
				//��ȡ����
				INT_PTR nWaitCount=m_WaitCollectItemArray.GetCount();
				m_wCollectItem=m_WaitCollectItemArray[nWaitCount-1];

				//ɾ������
				m_WaitCollectItemArray.RemoveAt(nWaitCount-1);

				//������Ϣ
				DWORD dwSocketID=(m_pBindParameter+m_wCollectItem)->dwSocketID;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}
		}
		else
		{
			//ɾ���ȴ�
			for (INT_PTR i=0;i<m_WaitCollectItemArray.GetCount();i++)
			{
				if (wBindIndex==m_WaitCollectItemArray[i])
				{
					m_WaitCollectItemArray.RemoveAt(i);
					break;
				}
			}
		}

		//��������
		CMD_CS_S_ServerRemove ServerRemove;
		ZeroMemory(&ServerRemove,sizeof(ServerRemove));

		//ɾ��֪ͨ
		ServerRemove.wServerID=pBindParameter->wServiceID;
		m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_REMOVE,&ServerRemove,sizeof(ServerRemove),0L);

		//ע������
		m_GlobalInfoManager.DeleteServerItem(pBindParameter->wServiceID);
	}

	//�㳡����
	if (pBindParameter->ServiceKind==ServiceKind_Plaza)
	{
		//��������
		WORD wBindIndex=LOWORD(dwSocketID);
		tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

		//ע������
		m_GlobalInfoManager.DeletePlazaItem(pBindParameter->wServiceID);
	}

	//�����Ϣ
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GC_INSERT_MATCH_SIGNUP_USER:			//�����������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(DBO_GC_Insert_Match_SignUp_User));
			if (wDataSize!=sizeof(DBO_GC_Insert_Match_SignUp_User)) return false;

			bool bIsInsertSuc = false;
			WORD wGroupID = 0;
			DBO_GC_Insert_Match_SignUp_User * pInsertMatchSignUpUser = (DBO_GC_Insert_Match_SignUp_User *)pData;

			if (pInsertMatchSignUpUser->bIsSignUpSuc == true)
			{
				printf("size of matchdata %d\n", m_vMatchData[pInsertMatchSignUpUser->nMatchID - 1].size());
				char szMsg[100];
				sprintf( szMsg, "�û�id=%d����������=%d,�ⳡ��ǰ����=%d,������=%d\n",pInsertMatchSignUpUser->dwUserID,pInsertMatchSignUpUser->nMatchID,m_vMatchData[pInsertMatchSignUpUser->nMatchID - 1].size(),pInsertMatchSignUpUser->wEnrollmentFee);
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

			//��������
			WORD wItemCount=wDataSize/sizeof(DBO_Load_Match_Config_Item);
			DBO_Load_Match_Config_Item * pGameItem=(DBO_Load_Match_Config_Item *)pData;
			m_MatchConfigItemList.clear();
			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				//��������
				tagMatchConfigItem MatchConfigItem;
				ZeroMemory(&MatchConfigItem,sizeof(MatchConfigItem));

				//��������
				MatchConfigItem.nStartTime = pGameItem->nStartTime;
				MatchConfigItem.nEndTime = pGameItem->nEndTime;
				MatchConfigItem.nApplyCost = pGameItem->nApplyCost;
				MatchConfigItem.nGameTime = pGameItem->nGameTime;
				MatchConfigItem.nMatchNum = pGameItem->nMatchNum;
				MatchConfigItem.nMatchUserCount = pGameItem->nMatchUserCount;
				MatchConfigItem.nMatchType = pGameItem->nMatchType;
				lstrcpyn(MatchConfigItem.szMatchName,pGameItem->szMatchName,CountArray(MatchConfigItem.szMatchName));
				printf("������ʼʱ��%d,��������=%d\n",MatchConfigItem.nStartTime,MatchConfigItem.nMatchType);
				//�����б�
				m_MatchConfigItemList.push_back(MatchConfigItem);
				pGameItem++;
			}
			return true;
		}
	case DBO_GR_GET_SQL_TIME:
		{
			ASSERT(wDataSize==sizeof(DBO_Get_Sql_Time));
			if (wDataSize!=sizeof(DBO_Get_Sql_Time)) return false;

			//��������
			DBO_Get_Sql_Time * pSqlTime=(DBO_Get_Sql_Time *)pData;
			time_t SqlTime = StringToDatetime(pSqlTime->szTime);
			time_t now = time(NULL);
			m_nDifferentTime = now - SqlTime;
			return true;
		}
	}

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	return true;
}

//ע�����
bool CAttemperEngineSink::OnTCPNetworkMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_REGISTER_PLAZA:	//ע��㳡
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_CS_C_RegisterPlaza));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterPlaza)) return false;

			//��Ϣ����
			CMD_CS_C_RegisterPlaza * pRegisterPlaza=(CMD_CS_C_RegisterPlaza *)pData;

			//��Ч�ж�
			if ((pRegisterPlaza->szServerName[0]==0)||(pRegisterPlaza->szServerAddr[0]==0))
			{
				//��������
				CMD_CS_S_RegisterFailure RegisterFailure;
				ZeroMemory(&RegisterFailure,sizeof(RegisterFailure));

				//���ñ���
				RegisterFailure.lErrorCode=0L;
				lstrcpyn(RegisterFailure.szDescribeString,TEXT("������ע��ʧ�ܣ��������ַ���롰�������������Ϸ���"),CountArray(RegisterFailure.szDescribeString));

				//������Ϣ
				WORD wStringSize=CountStringBuffer(RegisterFailure.szDescribeString);
				WORD wSendSize=sizeof(RegisterFailure)-sizeof(RegisterFailure.szDescribeString)+wStringSize;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REGISTER,SUB_CS_S_REGISTER_FAILURE,&RegisterFailure,wSendSize);

				//�ж�����
				m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

				return true;
			}

			//���ð�
			WORD wBindIndex=LOWORD(dwSocketID);
			(m_pBindParameter+wBindIndex)->wServiceID=wBindIndex;
			(m_pBindParameter+wBindIndex)->ServiceKind=ServiceKind_Plaza;

			//��������
			tagGamePlaza GamePlaza;
			ZeroMemory(&GamePlaza,sizeof(GamePlaza));

			//��������
			GamePlaza.wPlazaID=wBindIndex;
			lstrcpyn(GamePlaza.szServerName,pRegisterPlaza->szServerName,CountArray(GamePlaza.szServerName));
			lstrcpyn(GamePlaza.szServerAddr,pRegisterPlaza->szServerAddr,CountArray(GamePlaza.szServerAddr));

			//ע�᷿��
			m_GlobalInfoManager.ActivePlazaItem(wBindIndex,GamePlaza);

			//�����б�
			SendServerListItem(dwSocketID);

			//Ⱥ������
			m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true,0L);

			return true;
		}
	case SUB_CS_C_REGISTER_SERVER:	//ע�᷿��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_CS_C_RegisterServer));
			if (wDataSize!=sizeof(CMD_CS_C_RegisterServer)) return false;

			//��Ϣ����
			CMD_CS_C_RegisterServer * pRegisterServer=(CMD_CS_C_RegisterServer *)pData;

			//���ҷ���
			if (m_GlobalInfoManager.SearchServerItem(pRegisterServer->wServerID)!=NULL)
			{
				//��������
				CMD_CS_S_RegisterFailure RegisterFailure;
				ZeroMemory(&RegisterFailure,sizeof(RegisterFailure));

				//���ñ���
				RegisterFailure.lErrorCode=0L;
				lstrcpyn(RegisterFailure.szDescribeString,TEXT("�Ѿ�������ͬ��ʶ����Ϸ������񣬷������ע��ʧ��"),CountArray(RegisterFailure.szDescribeString));

				//������Ϣ
				WORD wStringSize=CountStringBuffer(RegisterFailure.szDescribeString);
				WORD wSendSize=sizeof(RegisterFailure)-sizeof(RegisterFailure.szDescribeString)+wStringSize;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REGISTER,SUB_CS_S_REGISTER_FAILURE,&RegisterFailure,wSendSize);

				//�ж�����
				m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

				return true;
			}

			//���ð�
			WORD wBindIndex=LOWORD(dwSocketID);
			(m_pBindParameter+wBindIndex)->ServiceKind=ServiceKind_Game;
			(m_pBindParameter+wBindIndex)->wServiceID=pRegisterServer->wServerID;

			//��������
			tagGameServer GameServer;
			ZeroMemory(&GameServer,sizeof(GameServer));

			//��������
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

			//ע�᷿��
			m_GlobalInfoManager.ActiveServerItem(wBindIndex,GameServer);

			//Ⱥ������
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,&GameServer,sizeof(GameServer),0L);

			//�����б�
			SendServerListItem(dwSocketID);

			//Ⱥ������
			m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true,0L);

			//����֪ͨ
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

//����״̬
bool CAttemperEngineSink::OnTCPNetworkMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SERVER_ONLINE:	//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_CS_C_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_C_ServerOnLine)) return false;

			//��Ϣ����
			CMD_CS_C_ServerOnLine * pServerOnLine=(CMD_CS_C_ServerOnLine *)pData;

			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//���ҷ���
			WORD wServerID=pBindParameter->wServiceID;
			CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(wServerID);

			//��������
			if (pGlobalServerItem!=NULL)
			{
				//��������
				CMD_CS_S_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//���ñ���
				pGlobalServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;

				//���ñ���
				ServerOnLine.wServerID=wServerID;
				ServerOnLine.dwOnLineCount=pServerOnLine->dwOnLineCount;

				//����֪ͨ
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine),0L);
			}

			return true;
		}
	case SUB_CS_C_SERVER_MODIFY:	//�����޸�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_CS_C_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_C_ServerModify)) return false;

			//��Ϣ����
			CMD_CS_C_ServerModify * pServerModify=(CMD_CS_C_ServerModify *)pData;

			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//���ҷ���
			ASSERT(m_GlobalInfoManager.SearchServerItem(pBindParameter->wServiceID)!=NULL);
			CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(pBindParameter->wServiceID);

			//�����޸�
			if (pGlobalServerItem!=NULL)
			{
				//���ñ���
				pGlobalServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGlobalServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGlobalServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGlobalServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGlobalServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGlobalServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGlobalServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGlobalServerItem->m_GameServer.szServerName));
				lstrcpyn(pGlobalServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGlobalServerItem->m_GameServer.szServerAddr));

				//��������
				CMD_CS_S_ServerModify ServerModify;
				ZeroMemory(&ServerModify,sizeof(ServerModify));

				//���ñ���
				ServerModify.wKindID=pServerModify->wKindID;
				ServerModify.wSortID=pServerModify->wSortID;
				ServerModify.wNodeID=pServerModify->wNodeID;
				ServerModify.wServerID=pBindParameter->wServiceID;
				ServerModify.wServerPort=pServerModify->wServerPort;
				ServerModify.dwOnLineCount=pServerModify->dwOnLineCount;
				ServerModify.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(ServerModify.szServerAddr,pServerModify->szServerAddr,CountArray(ServerModify.szServerAddr));
				lstrcpyn(ServerModify.szServerName,pServerModify->szServerName,CountArray(ServerModify.szServerName));

				//����֪ͨ
				m_pITCPNetworkEngine->SendDataBatch(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_MODIFY,&ServerModify,sizeof(ServerModify),0L);
			}

			return true;
		}
	}

	return false;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_USER_ENTER:		//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_CS_C_UserEnter));
			//if (wDataSize!=sizeof(CMD_CS_C_UserEnter)) return false;
			if (wDataSize!=sizeof(CMD_CS_C_UserEnter)) return true;

			//��Ϣ����
			CMD_CS_C_UserEnter * pUserEnter=(CMD_CS_C_UserEnter *)pData;
			pUserEnter->szNickName[CountArray(pUserEnter->szNickName)-1]=0;

			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//��������
			tagGlobalUserInfo GlobalUserInfo;
			ZeroMemory(&GlobalUserInfo,sizeof(GlobalUserInfo));

			//���ñ���
			GlobalUserInfo.dwUserID=pUserEnter->dwUserID;
			GlobalUserInfo.dwGameID=pUserEnter->dwGameID;
			lstrcpyn(GlobalUserInfo.szNickName,pUserEnter->szNickName,CountArray(GlobalUserInfo.szNickName));

			//������Ϣ
			GlobalUserInfo.cbGender=pUserEnter->cbGender;
			GlobalUserInfo.cbMemberOrder=pUserEnter->cbMemberOrder;
			GlobalUserInfo.cbMasterOrder=pUserEnter->cbMasterOrder;
			GlobalUserInfo.cbAndroid=pUserEnter->cbAndroid;

			//�����û�
			m_GlobalInfoManager.ActiveUserItem(GlobalUserInfo,pBindParameter->wServiceID);

			return true;
		}
	case SUB_CS_C_USER_LEAVE:		//�û��뿪
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_CS_C_UserLeave));
			if (wDataSize!=sizeof(CMD_CS_C_UserLeave)) return false;

			//��Ϣ����
			CMD_CS_C_UserLeave * pUserLeave=(CMD_CS_C_UserLeave *)pData;

			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT(pBindParameter->ServiceKind==ServiceKind_Game);
			if (pBindParameter->ServiceKind!=ServiceKind_Game) return false;

			//ɾ���û�
			m_GlobalInfoManager.DeleteUserItem(pUserLeave->dwUserID,pBindParameter->wServiceID);

			return true;
		}
	case SUB_CS_C_USER_FINISH:		//�û����
		{
			//��ȡ����
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			//����Ч��
			ASSERT((m_wCollectItem==wBindIndex)&&(pBindParameter->ServiceKind==ServiceKind_Game));
			if ((m_wCollectItem!=wBindIndex)||(pBindParameter->ServiceKind!=ServiceKind_Game)) return false;

			//���ñ���
			m_wCollectItem=INVALID_WORD;

			//�����л�
			if (m_WaitCollectItemArray.GetCount()>0)
			{
				//�л�����
				INT_PTR nWaitCount=m_WaitCollectItemArray.GetCount();
				m_wCollectItem=m_WaitCollectItemArray[nWaitCount-1];

				//ɾ������
				m_WaitCollectItemArray.RemoveAt(nWaitCount-1);

				//������Ϣ
				DWORD dwSocketID=(m_pBindParameter+m_wCollectItem)->dwSocketID;
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_USER_COLLECT,SUB_CS_S_COLLECT_REQUEST);
			}

			return true;
		}
	}

	return false;
}

//Զ�̷���
bool CAttemperEngineSink::OnTCPNetworkMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SEARCH_CORRESPOND:	//Э������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_CS_C_SearchCorrespond)) return false;

			//������Ϣ
			CMD_CS_C_SearchCorrespond * pSearchCorrespond=(CMD_CS_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//��������
			CMD_CS_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//����û�
			CGlobalUserItem * ResultUserItem[2];
			ZeroMemory(ResultUserItem,sizeof(ResultUserItem));

			//���ñ���
			SearchCorrespond.dwSocketID=pSearchCorrespond->dwSocketID;
			SearchCorrespond.dwClientAddr=pSearchCorrespond->dwClientAddr;

			//�����û�
			if (pSearchCorrespond->dwGameID!=0L)
			{
				ResultUserItem[0]=m_GlobalInfoManager.SearchUserItemByGameID(pSearchCorrespond->dwGameID);
			}

			//�����û�
			if (pSearchCorrespond->szNickName[0]!=0)
			{
				_tcslwr(pSearchCorrespond->szNickName);
				ResultUserItem[1]=m_GlobalInfoManager.SearchUserItemByNickName(pSearchCorrespond->szNickName);
			}

			//���ý��
			for (BYTE i=0;i<CountArray(ResultUserItem);i++)
			{
				if (ResultUserItem[i]!=NULL)
				{
					//��������
					WORD wServerIndex=0;

					//���ҷ���
					do
					{
						//���ҷ���
						CGlobalServerItem * pGlobalServerItem=ResultUserItem[i]->EnumServerItem(wServerIndex++);

						//��ֹ�ж�
						if (pGlobalServerItem==NULL) break;
						if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

						//��������
						WORD wIndex=SearchCorrespond.wUserCount++;

						//������Ϣ
						SearchCorrespond.UserRemoteInfo[wIndex].cbGender=ResultUserItem[i]->GetGender();
						SearchCorrespond.UserRemoteInfo[wIndex].cbMemberOrder=ResultUserItem[i]->GetMemberOrder();
						SearchCorrespond.UserRemoteInfo[wIndex].cbMasterOrder=ResultUserItem[i]->GetMasterOrder();

						//�û���Ϣ
						SearchCorrespond.UserRemoteInfo[wIndex].dwUserID=ResultUserItem[i]->GetUserID();
						SearchCorrespond.UserRemoteInfo[wIndex].dwGameID=ResultUserItem[i]->GetGameID();
						lstrcpyn(SearchCorrespond.UserRemoteInfo[wIndex].szNickName,ResultUserItem[i]->GetNickName(),LEN_NICKNAME);

						//������Ϣ
						SearchCorrespond.UserRemoteInfo[wIndex].wKindID=pGlobalServerItem->GetKindID();
						SearchCorrespond.UserRemoteInfo[wIndex].wServerID=pGlobalServerItem->GetServerID();
						lstrcpyn(SearchCorrespond.UserRemoteInfo[wIndex].szGameServer,pGlobalServerItem->m_GameServer.szServerName,LEN_SERVER);

					} while (true);
				}
			}

			//��������
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_REMOTE_SERVICE,SUB_CS_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}

	return false;
}

//�������
bool CAttemperEngineSink::OnTCPNetworkMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_C_SYSTEM_MESSAGE:	//ϵͳ��Ϣ
		{
			//����֪ͨ
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_SYSTEM_MESSAGE,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_PROPERTY_TRUMPET:  //������Ϣ
		{
			//����֪ͨ
			m_pITCPNetworkEngine->SendDataBatch(MDM_CS_MANAGER_SERVICE,SUB_CS_S_PROPERTY_TRUMPET,pData,wDataSize,0L);
			return true;
		}
	case SUB_CS_C_VIP_TRUMPET:
		{
			//����֪ͨ
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
					//printf("%d����=%d\n",j,m_vMatchData[m_nCurrentMatchID - 1][i][j].cbRank);
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
			printf("�����ݿ����ʱ��=%d,ʣ��ʱ��=%d\n",m_nDifferentTime,stMatchScoreUpdate.nSecond);
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
				printf("�����䷢�ͷ����ı�id=%d\n",m_vMatchGameServerID[m_nCurrentMatchID - 1][i]);
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

			// ����������Ϣ, �ҳ��û���Ϣ
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

//�����б�
bool CAttemperEngineSink::SendServerListItem(DWORD dwSocketID)
{
	//��������
	WORD wPacketSize=0L;
	POSITION Position=NULL;
	BYTE cbBuffer[SOCKET_TCP_PACKET];

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INFO);

	//�ռ�����
	do
	{
		//��������
		if ((wPacketSize+sizeof(tagGameServer))>sizeof(cbBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,cbBuffer,wPacketSize);
			wPacketSize=0;
		}

		//��ȡ����
		tagGameServer * pGameServer=(tagGameServer *)(cbBuffer+wPacketSize);
		CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.EnumServerItem(Position);

		//��������
		if (pGlobalServerItem!=NULL)
		{
			wPacketSize+=sizeof(tagGameServer);
			CopyMemory(pGameServer,&pGlobalServerItem->m_GameServer,sizeof(tagGameServer));
		}

		printf("%s %s\n", pGameServer->szServerName, pGameServer->szServerAddr);

	} while (Position!=NULL);

	//��������
	if (wPacketSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_INSERT,cbBuffer,wPacketSize);

	//�������
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_FINISH);

	return true;
}

//���䷢��
bool CAttemperEngineSink::SendDataToGame(WORD wServerID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//���ҷ���
	CGlobalServerItem * pGlobalServerItem=m_GlobalInfoManager.SearchServerItem(wServerID);
	if (pGlobalServerItem==NULL) return false;

	//��ȡ����
	WORD wBindIndex=pGlobalServerItem->GetIndex();
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//��������
	DWORD dwSocketID=pBindParameter->dwSocketID;
	m_pITCPNetworkEngine->SendData(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//�û�����
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
			//printf("�����һ������ʱ��=%d,��������=%d\n",iter->nStartTime,iter->nMatchType);
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

//����֪ͨ��Ϣ �����еı�����ҷ���ͳһ����Ϣ, ���ͳһ����Ϣ�Ľṹ��С��wAttachDataSize����
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

//��ÿ��GameServer���Ͷ�Ӧ��User����Ϣ
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

//���ͱ���������Ϣ��������
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
					//��ӽ���������Ϣ
					stUserInfo.wVectorSize++;
				}
			}
		}
		wSendDataSize = sizeof(WORD)*2 + stUserInfo.wVectorSize * sizeof(tagMatchEndInfo);
		printf("���ͱ����������wSendDataSize=%d, ServerID is %d\n",wSendDataSize, m_vMatchGameServerID[m_nCurrentMatchID - 1][i]);
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

	time_t t_ = mktime(&tm_); //�Ѿ�����8��ʱ��  
	
	return t_; //��ʱ��  
}

void CAttemperEngineSink::OnRecord(CString strMsg)
{
	CString strFileDlgPath;
	TCHAR szModuleDirectory[MAX_PATH];	//ģ��Ŀ¼
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
	sprintf( szTime, "����::%d:%d:%d:%d\r\n",sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
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
