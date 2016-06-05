#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////
//ʱ���ʶ

#define IDI_LOAD_ANDROID_USER		(IDI_MAIN_MODULE_START+1)			//������Ϣ
#define IDI_REPORT_SERVER_INFO		(IDI_MAIN_MODULE_START+2)			//������Ϣ
#define IDI_CONNECT_CORRESPOND		(IDI_MAIN_MODULE_START+3)			//����ʱ��
#define IDI_GAME_SERVICE_PULSE		(IDI_MAIN_MODULE_START+4)			//��������
#define IDI_DISTRIBUTE_ANDROID		(IDI_MAIN_MODULE_START+5)			//�������
#define IDI_DBCORRESPOND_NOTIFY		(IDI_MAIN_MODULE_START+6)			//����֪ͨ
#define IDI_LOAD_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+7)			//ϵͳ��Ϣ
#define IDI_SEND_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+8)			//ϵͳ��Ϣ
#define IDI_LOAD_SENSITIVE_WORD		(IDI_MAIN_MODULE_START+9)			//�������д�
#define IDI_DISTRIBUTE_USER		    (IDI_MAIN_MODULE_START+10)			//�����û�
#define IDI_CHECK_STOCK				(IDI_MAIN_MODULE_START+11)			//���ʱ���ȡ��д��
#define IDI_GET_USER_LUCKY			(IDI_MAIN_MODULE_START+12)
#define IDI_DISTRIBUTE_PRECETABLE   (IDI_MAIN_MODULE_START+13)			//���·�����������
#define IDI_CLOWN_FAKE_MSG			(IDI_MAIN_MODULE_START+14)			//С����н���Ϣ
#define IDI_READ_CONFIG				(IDI_MAIN_MODULE_START+15)			//��ʱ��ȡ����
#define IDI_CLOWN_BIG_REWARD		(IDI_MAIN_MODULE_START+16)			//�㲥�޽�
//#define IDI_READ_MATCH_CONFIG		(IDI_MAIN_MODULE_START+17)			//��ȡ��������
//#define IDI_TIMER_CHECK_INTERVAL	(IDI_MAIN_MODULE_START+18)			//����ʱ����
//#define IDI_MATCH_BEGIN_REMINDER	(IDI_MAIN_MODULE_START+19)			//������ʼ����ʱ

#define IDI_MATCH_SIGN_UP			(IDI_MAIN_MODULE_START+20)			//����ʱ��
#define IDI_MATCH_TIME				(IDI_MAIN_MODULE_START+21)			//����ʱ���Ƿ�ʼ
#define IDI_MATCH_GET_USERSCORE		(IDI_MAIN_MODULE_START+22)			//��ʱ��ȡ�û�����

//////////////////////////////////////////////////////////////////////////////////
//ʱ�䶨�� ��

#define TIME_LOAD_ANDROID_USER		600L								//���ػ���
#define TIME_DISTRIBUTE_ANDROID		8L									//�����û�
#define TIME_REPORT_SERVER_INFO		30L									//�ϱ�ʱ��
#define TIME_DBCORRESPOND_NOTIFY	3L									//����֪ͨʱ��
#define TIME_LOAD_SYSTEM_MESSAGE	3600L								//ϵͳ��Ϣʱ��
#define TIME_SEND_SYSTEM_MESSAGE	10L								    //ϵͳ��Ϣʱ��
#define TIME_LOAD_SENSITIVE_WORD	5L									//�������д�ʱ��
#define TIMER_CHECK_STOCK_OP		20000L								//���ʱ���ȡ��д��
#define TIMER_GET_USER_LUCKY		20000L								//�û�����ֵʱ��
#define TIME_DISTRIBUTE_PRETABLE	20L									//�������д�ʱ��
#define TIME_SEND_CLOWN_FAKEMSG		600000L								//����С�����Ϣ��ʱ��
#define TIME_READ_CONFIG			60000L								//��ʱ��ȡ����
#define TIME_MATCH_SIGN_UP			5000L								//��������ʱ��
#define TIME_MATCH_DURATION			5000L								//��������ʱ��
#define TIME_MATCH_GET_USERSCORE_INTERVAL	5000L						//��ȡ����ʱ����
//////////////////////////////////////////////////////////////////////////////////

char *  g_pszFakeName[] = {"��ս�پ�","�������","��˧��������","���С��ȹ","��������","����������","Ц����˪","�����¸ɱ�","����֮��","���д���","���˾;���","��ʧ���˺�","а������ǹ","�ڶ��ڰ��","������ɽ�ɻ�","��ȻҰ��˧��","һ��ͺ�˧","���ڲ�����","����ι����","���ɸ�","�ܰ��벻�ȶ�һ��","ushdhdebgd","bvddfghhjj","augvss","dghjjhg","142588","Hggtvh","��һ��","���ʲ�","hjij66663","dfghhbbbn","cvgrdd","vuhghjj","0459zd","����","������Ц��ֹ","yvdexm","suggshsh","ydrrdrrr","bxbdveb","okjkohk","jpz011","jpz009","�ܰ�����","6647568","duliang","Hsshshsjs","zg747606","Csydhsshshs","��������","ghjeeff","����999","369077","nnjxjje","�������ɿ���","�������","ˮ���߲ʺ�","sdjp18","�ʽ�С����","9467852","Ywijhmh","���ʮ��","hdghxvgdv","dududu","dtghhjjn","Zhengjing6","���鲶��","Ԥ�深���ƽ�","shouldhave","��·��ģ��","��","ffuvuc","nimala","֪�㳣��","С��8287336","�����ޱ�","ai5215","wqasewa","lw7777lw","Qsadsrff","ѧ��������","��������","������ӳ�","����֪����","�����İ�","ǳ��΢ױ","������Ϣ","d���Ļ���","��ϲ����","�����ǵĴ�","����Ҵ���","���ڵ���ؼBSW","��β����","92������","y1����Ҳ�ܻ�","������΢Ц","����û�����","���������","������","��ʽ��ؼ�µ�","_�����Ѳ�����","����","����","����С����","�両����","adsferf","�ж�����ǿ","��������","ʦ̫�����","ǳ���Ż���","�뵺�Ƶ�","���˸�HelloKitty","����į�ͺ�","��������","���������","����̫ӵ��","Ľ����ʺ��","�����˵İְ�","׷���Ҹ���","�����貽","޹�²ݵ�����","˵���һ�Ц","���౦��","�ո���û�е�","ɽ���еļ�Ʒ","��ʹ�ҵ���","����ס�Ķ���","����������","�ݺ�ƽ��","˹�˶���","���ʹҴ�","ϧЦ������","����������","���ݲп�","īС��","���˾͹���","īī","��¥������","�������","һ��ͺ�˧","����176","��������","ĺɫ΢��","ָ����ӣ��^_^","�ٸ�����","��������","����B����","����һö","��ˮ�̺�","��Ҫ�Լ���","�����ü","������","��С��","Ǿޱ����","���ľͺ�","�������","���ڶ���","���ľͺ�","���ڶ���","��Цؿ����","��������̧��","�����İ�����","[ѧ��������]","����լ��","��Ҫ��ô��ͣ","���ľͺ�","��������","ӵ����","�����Ӱ","���³����","���ٲ���سɻ�","�������һƬ��","����������������","�㲻������","����������","��굽������","����ؼ����ؼ��˧","���ڱ�������ս","�û���꼾","�͵��⸻˧","÷��0012","���������","û��������ק","����װ�ƴ��ҷ�","������ѧ��","װ�Ʋ��ʺ���","��Ǯ��������","��������","���ǹ�","����С��","����������","��ͼ����Լ�","����������","��Ӯ��ͣ","ȫ��˧�ȴ���","���ʲ�ζ��","������������","��̫����","����������","Ը��һ����","�������α","������888","��ү��","С����ɭ","�Ұ�������","����ҹ̫��","����ؼĪ����","��ң����","�����ײ���","����ʹ��","��ս����","Ϊ�Լ�����","���ı��˽���","椸����̳���","�ջ�����","�����������","ҹ����","ͯ�����Ǽ�","��Ȳ���������","��Լ����","�˹���;","��������","��ί���һ����","��������","��ֹ�ŵ�","�ݰݰ���","����Ϊֹ","����ϰ����Ȼ","һ�׼򵥵ĸ�","��ռŮ����","��Ȼ����","��ˮ�޺�","����Ժ����","�͵���Ĭ��","��������","��ˮ�е�ѩ","����ʽ����","�ݺݰ�","���������","С��","�������","����������","���˼��껪","������","һ���į�Ļ�","˼����һ�ֲ�","�Ĳ�����ʹ","90��ح˧��","��������ʹ","ҹ��������","��ʵ̫��ʵ","����˱���","��㯵���;","ע��һ����","�黰��ʦ","Ц������","����Ե��","��˿ŮHumane","������΢Ц","����˭����˵","�Ȳ�������","�ü�įԶ��","��������","_ǳ��ɫDe��","ˮ���Ƶ�","����Ҹ�","��������ϥ","̤ѩѰ÷","�ý���","��Ӱ���","�к�����","�쾵����","������¥","���߲�����","�����ޱ�","��ˮ��Ƽ","��������","������","�����޺���","������ˮ","���販","����׷�Ұ�����","������","����ޱ��","�������","�غƴ�","������Ǽ������","��į����","ҳ��","����ʫ��","ҹ�Ĳ�ƽ��","�ź����Ƕ���","����Ѷ�","���������","�������","Ľ������","����ʽ��ˮ","����Ҳ�᲻��ǿ","�����˾�Сɧ��","С���˴��ǻ�","������������","��ô����ôԶ","�������˸���","�þӹµ�������","�Ӳ��͵�","���Ӵ��","΢΢һЦ��쳾","��Щ��������","������Ĩ��","С��ֻ��������","��˹��","�����","��������","���ڼ�į","����C��","��ή��ڻ�","���겻��ҵ","��������","͵�Ĵ��","����","�ٻ�����İ·","β��İ�","����ʿ��ȱ��","�컯Ū��","��С��","δ�����","������ϲ��","������ҵ������","�����ҵ�����","�Բ�����޴���","����������","100������","������˾��ǰ�","��Ӿ����","�ƽ�����","˧������¥","����de����","������99","��ţû��ţ","���ڻ���","Royal����è","С����","���������","��ѽ��ѽӴ","����ţ�����","��Ӯ�Ż�ƴ","ת�ð������","����wifi������","����޿�����","Ц��������","��������","���������ֺǺ�","�ռ���ǿ","����վֱ��ſ��","��������ν��","��Ϊ�Լ�����","�Ƽ�ɵ�","�ഺ���԰�","��4ק","˵��̫����","����Э��᳤i","����������","�ݰ˺Ⱦ�˵ʮ��","�����Ҹ�Ǯ��","����Ҳ����","������","÷������","�����İ�","�¶�","��ըƤ����","��βů���׳���","ת��˵�ټ�","�ҹ�����������","��ȱ������","װ�ƽ翸����","һ��С����","����Ц�쳾","��������ӭ��","ʫ�Ƴ�����","�Ѿ��ٷ�","������","ugugugw","��ݼ����","njf566","���ƴ���","jedndbj","liangshanbo","tugugug","hsjsjhsj","gjefduf","3ed4dx","wc80077","hvcdsf2fhjjj","eudjejm","dounimei","����Ϯһ��","gbthghgihhh","sdjp16","ddtyyuff","junjun520","dhxydgennx","��������","vyvyvubvb","��������","udndjdjfidj","�¹��ȷ������","����","����13","dhfjshs","С������","123456ĭĭ","hduderh","HRsitser","jxx41115","ytfghrd","nishikou44","sgxxhh","qtjmxj","hdhd222","hcijfnkddd","eedddsjjejsee","qianerdenger","wwhuhddhh","�Խ���ͷ","xfghrdcjkkm","liuyangming8","zo747nii","uddjjd2222","������Ȼ","��ī����Ŷ","��÷��������","��^^��","����è","��ӭɧ��","һ��±��_","ι˵����","���ֹ���û��","��С��^^","��Ȼ������","�ϵ��Ǹ��","��ĸ�����","���Ƕ�����˭","������β��","��ˮ�����˶�","����ɽ��ƨؼ","���̿���","ʺ������","32����Ȣ��","�ñ��ϲ���","��������ϧ","��ĸ������","���ӿ�","��������","����ָ����","��������","��һ�����̻�","�����ȷ��","�����߲�","�䰮","����߲���","�ĵ�δ��","����·�˼�","��ʵ�Ҳ�ɵ","huxian888","xsssssj","zl1382","���˰���Ӯ","¬��ô123","����ж��","iuy9688","abcdbb","wz199611","ҹ������","dq2563","ssseezzz","zo8787","jzk130708","tu908553","gyffd3555","xgfkfhg","hnhfguuu7","mnbhjkl123","bnnhgtffff","123djj","huxian777","accdaaaa","�Ͻ���","����������","wwwewweq","dq13468","123djm","023123vvkfjrt","x814420053","zzzjjj88","Gujnnj","kkjnxkbbji","uvf67i8","�����123","Chu����̫����","������ʵ1","rgijgg","jsnsnsnxs","�����Ǹ�ү����","��������","rddgg448","��������","vbhgyunjik","jjdjdde","fghjkkhj","bhdhvsge","hdbxbdjdn","vgy123580","nimeim","������123","Zhangge","e888888e","kkkeerp","ffgtgtgw","nsnsnsnd","���˶���yyy","hgfu8765","û������i","��ս������","123z4g5m","r561826574","zhaihao","cccxxxz","fxfgg1478","nsnwwww","wc88imdii","jxhgdgd","xcv11111","ygffvh","jw77178ej","aiaiaiai","tuvf668","bdu6wh8i","��õ�","jjjjnjnnh","ftygfhuh","ycyfhchvj","jspeibipo1","hbdbjdhsh","��������464","gddyjh","����ը��","yangyan","fig8f8f8","�Ʒ�38","4ch6bj","eraaatj","gsggshhsh","�ҾͲ�������","jssnsnseee","33441122","srffgvhhj","������ĥ��","��������","zsyzjl123","����Ҳ","ghjcgcg","xbhdhdjzj","rgyihf","����008","dtfhvhh","4r5668","jkajtdnw","rfguiiooll","cyydgj","�㿩logo","˵����ָ�","����С����","��¥��������","6869583","12rsftgg","nsnsnsss","���0317","2271343231","��̲�ȼ�","fdsuhggji","����ʹ","�����ල�ֵ�","�������»�","xiaowan","����8","dingggnajhg","jhffddhhj","bdhchhfyd","С��ֽ��J","gucgju","d666666d","߼��","�÷����ü���","jyfddt","�ް���","С������","gugugyyj","Ŭ���˾����˰�","gghhjnv","Dingsidang","��Ŷ��һ��","w24j12x11","gdjjjrbfd","dfvyyhj","���Ը߸����","jdhgejdvd","teghehdh","787yuyy7t6y7","Xnxidsn","15515515wu","a895558","pjantgan","jsjsjssjshsh","tyu677","jhdhdhfn","12315q","��������˵11","cm����Ѫpo","xvxgdgd","�ݼ���","dmwgua","vy316419js","���","ttd3467","��Ů��ɫ","king��˴���","����ǧ����","dhhvdgh","������12323","jdhfhdjhdve","�Ŀ���ıni","����","fyhbhh","dssryhg","lkkijo","�����´���","lovemm","ftgmjg","dgdgueuege","�л���","��Ҿ��÷·��","qingang1","ysgeywgjsh","d12332","a8957480","gfh654","vjghcf","jdhdbbdbddbn","wzh530","qwevbjk123","��������","bae111����","huzijjdajjda","Xiao��","dmtptdt","Bxjsne","Ҳ���Ц�Ǻ�","hcheue369","�Ծ�����","xl461126464","gdjhdtuf","yiyuhen","asz898","�㽭����","w7777lw","w6666lw","xtrxtrysss","ss1314520","jjuuiju","ͬ��ɽ��","��ܽ��","yyyyyyying","etwyug","ֻ������","������15745","fhfbfcbdnd","548064","������a","��aaaaaa","gggfdee","111111w","wyx333","jxl1314","12377y","����DJ","jhcrty","rrttyyuuii","����998","vcdtgb","����666","��־","dtugdrj","ghssef","�����","nhfdry","n00544","hbvfdewsxz","hsgech","ghggggb","jsnsmddd","hgtyhg6","�ڶ������","���Ų�","11111f","seezddeewe","dongzi5","�ҷ�����","�ո��޵а汾","a15fgcfff","�仰��","hhhyivkv","gghgggh","���","yhgghl","qwsddff","dongzi4","252525nn","yyegggdggh","frgji5536","a2863115082","usjejesww","tszxhs","w6719625","zxcvbnmlinb","wrjfiydi","a65001234","�����11","�������110","a15070041","dongzi3","fmfjhhjyfj","������Ҳ","ר�Ҵ���","uoa5866999","���������","������","jhjhuuhjj","778899oo","aaz1234","������","djjjjjkk","hahaasre","������","u��v����","���ܷǹٷ�","puj2888856","Hdjfjfjfjf","139968146","agily16","777777f","xx09616","yesyes777","sssszxd","a15070041","hai1234567","ghjhvbn","q6445320","hehe2520m","dongzi1","lililiuliu","�ҾͲ���֪11","��������","hhhhh222222","����LadyGaga","FZMCZD","tchvds","�ż������","59383566kk","mo1363825","lukais62868","������������","490677","С�����","�µ�Ӣ��","�ȲȲȲ�c","dfghhjjv","���������","juslljz","�ǵò鿴����","xxxdfrs","wzh158","tyffggtdsfg","agily15","djdkww","�۷�ؼ����","wangyui96","huij6366","a15070041kf","jhdhfmf","jdshh34","yuan137094","�ܹؼ�����","huozhi","������Ļ�","ghgffgniu6fg","ngfmjuggr","ghjjgvh","����","�����￩��","dghffd","a650se","cdtdff","520336","�����о�","666tsfwftstw","������ϻ�ba","kkriben","cvhhhyhh","hhcdryjj","������","�½���8","weqasdewa","ddfhvffg","xdfvvcf","Hejhzhdd","555555f","julijuzu","123456wasn","agily14","huzijjdajj","axuda6500","��Ϣ��Ϣxx","jdhhhrhj","888888x","�������","����","�Ҿ�ȥ����11","ajdjjdjd12","����","ijuyttti","ˮ������","mdkjdhffj","�����","610289","fgt123","�������˵�","zk747606","jlh814","�����jiqiren","������","xuziyuan666","���������ֻ�","С��123","�ᵶ����ЦΪ","opl87072","987��","liuyang660","243320242","yyysss14","gplwun","jqx123","guigui12","woshishuiq","hzhhbshsh","529077","binbinbin","wccc8207","�ȿں���һ��","hdbfhgfg","shijie789","890890a9","wywy51","mg528885","uhggff","dkeem433","fresrew","ˮ�����","fhjjfg","snbxhxjzjjsks","С�����е���","Ehejhsn","yoyo52000","�Ʒ�֪��X","wcc88207","hgffhjf","ά����D","Сΰ����","A497396","jpgmwjat","7845124","6tuyffhjj","fghjcfhvcgb","jt12356","�Ը�kk","fhfufhfy","jdbh678","qylpcb","ghjkkkkggjk","hjtpgaujg","woshini","ɶʱ���Ǹ�ͷ","xfso222","kk284628","iiii646464","zu74760","m96n649","wc82078","bhjycvg","������ͼ����","qtmkwn","SisterHeRui","xgfhff","xk19890526","xbjdbjdjj","���ǲ�����","mwtppj","������","y52669","sdjp169","ak47ak","�������","����˺�","ye12356","lilipen","s4gf123","��Ӫÿƽ��","cbgdjc","�����ڴ�","1987118al","yu����yu","343591037b","dbbxdee733","�����","����","�����Բ���","iijoll","cy2234","�����Ľ���i","Ѧ�ճ�","ˮ������","1q1q2w3e","ι��һ��","bb142536","tgtddhr","grhjuhty","23748392","gdfigc","jyujenwt","yangjjj111","a15070041","3wangli","���ɺÿ���","��ҵİ취","lzc8889","456712aq","wmwmpk","ˮ����","jiagang002","������7","Hwؼ����","dd14253","A15132","wanglli2","dcjkfesxvh","QsDsddsf","RMB8000","bhft5678","Ҫni�����","wang7409","mwpvajt","Yufyvyyvu","����","buyummm","a1507004174","bffhihy","���ϵ��ܷ�","260766","������ɱ��","kughjyg","�����ָ�","namjdjekdmd","�Ұ���1314","��ʮ����","���ܷ��㲻","ˮ���","hdgdgr","ly510125","a91271","978568","zjsnjzjz","rir9ed","����Ĺ淶","��������","556680009","���Ľ���","sdfg168","890223nijuan","qwsjjs","����007","a15070804","eeieokfd","���ϵ���","a2010222","d�ȴ�����ô��","Kl7789jsj","xxdfghj","q866052","z7203313","1wangli","hua��2222","long66635","ghjjjjjjjf","�󽭴�","345678ii","ghzjs556889","cc806500","jhhgbnjjj","������","�����3","yffujg","ly520061","qa866052098","ygfgj999","aa0708043","awangli","ˮ��һ","Fe0909","ppplllaa","С����","dmtpwjt","��������","ujjhgcvh","sfvezfd","Ŷ����Ŷ¥Ŷ","jfkfkfmff","poiioooo","��һ�ݻ��ۻ�","ptmwmahx","�ڴ������","ydhh123","shgdyew"};


//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//������
	m_pNormalParameter=NULL;
	m_pAndroidParameter=NULL;

	//״̬����
	m_pInitParameter=NULL;
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIAttemperEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pIGameServiceManager=NULL;
	m_WaitDistributeList.RemoveAll();

	//��������
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;
	m_pIDBCorrespondManager=NULL;

	//��������
	ZeroMemory(&m_DataConfigColumn,sizeof(m_DataConfigColumn));
	ZeroMemory(&m_DataConfigProperty,sizeof(m_DataConfigProperty));

	//��������
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

	// ��������
	m_bMatchSignUpMark = false;
	m_bIsMatchTime = false;
	m_nCurrentMatchID = 0;
	m_wUserCntInGroup = 8;
	printf("constructor m_nCurrentMatchID\n");

	m_vMatchUserInfo.resize(100);

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
	//ɾ������
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//ɾ������
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//��������
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

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IMainServiceFrame,Guid,dwQueryVer);
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//����Ϣ
	m_pAndroidParameter=new tagBindParameter[MAX_ANDROID];
	ZeroMemory(m_pAndroidParameter,sizeof(tagBindParameter)*MAX_ANDROID);

	//����Ϣ
	m_pNormalParameter=new tagBindParameter[m_pGameServiceOption->wMaxPlayer];
	ZeroMemory(m_pNormalParameter,sizeof(tagBindParameter)*m_pGameServiceOption->wMaxPlayer);

	//���û���
	if (InitAndroidUser()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if (InitTableFrameArray()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ýӿ�
	if (m_ServerUserManager.SetServerUserItemSink(QUERY_ME_INTERFACE(IServerUserItemSink))==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_AndroidUserManager.StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//����ʱ��
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

	//��ʱ�������д�
	m_pITimerEngine->SetTimer(IDI_LOAD_SENSITIVE_WORD,TIME_LOAD_SENSITIVE_WORD*1000L,TIMES_INFINITY,NULL);

	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_PRECETABLE,TIME_DISTRIBUTE_PRETABLE*1000L,TIMES_INFINITY,NULL);
	
	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_USER,10*1000,TIMES_INFINITY,NULL);

	m_pITimerEngine->SetTimer(IDI_CHECK_STOCK,TIMER_CHECK_STOCK_OP,(DWORD)(-1),NULL);
	m_pITimerEngine->SetTimer(IDI_GET_USER_LUCKY,TIMER_GET_USER_LUCKY,(DWORD)(-1),NULL);
	//2008�ǳ���������Ϸ����
	if (m_pGameServiceOption->wKindID == 2008)
	{
		m_pITimerEngine->SetTimer(IDI_CLOWN_FAKE_MSG,m_wFakeAnnounceTimeInterval,(DWORD)(-1),NULL);
		m_pITimerEngine->SetTimer(IDI_READ_CONFIG,TIME_READ_CONFIG,(DWORD)(-1),NULL);
		m_pITimerEngine->SetTimer(IDI_CLOWN_BIG_REWARD,5000,(DWORD)(-1),NULL);
	}
	//2001�ǲ�������Ϸ
	//if (m_pGameServiceOption->wKindID == 2001)
	//{
	//	m_pITimerEngine->SetTimer(IDI_READ_MATCH_CONFIG,60000,(DWORD)(-1),NULL);
	//	m_pITimerEngine->SetTimer(IDI_TIMER_CHECK_INTERVAL,60000,(DWORD)(-1),NULL);
	//}

	//����ʱ��
	m_pITimerEngine->SetTimer(IDI_MATCH_SIGN_UP, TIME_MATCH_SIGN_UP, (DWORD)(-1),NULL);
	//m_pITimerEngine->SetTimer(IDI_MATCH_TIME, TIME_MATCH_DURATION, (DWORD)(-1), NULL);

	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//������Ϣ
	m_pInitParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;

	//��������
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;

	//������
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//ɾ������
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//ɾ���û�
	m_TableFrameArray.RemoveAll();
	m_ServerUserManager.DeleteUserItem();
	m_ServerListManager.ResetServerList();
	m_WaitDistributeList.RemoveAll();
	
	//ֹͣ����
	m_AndroidUserManager.ConcludeService();

	//ֹͣ����
	if(m_pIGameMatchServiceManager!=NULL)
		m_pIGameMatchServiceManager->StopService();

	//�����Ϣ����
	ClearSystemMessageData();

	//��λ�ؼ���
	m_WordsFilter.ResetSensitiveWordArray();

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_CONNECT_CORRESPOND:		//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case CT_LOAD_SERVICE_CONFIG:	//��������
		{
			//��������
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_PARAMETER,0L,NULL,0L);

			//�����б�
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_GAME_COLUMN,0L,NULL,0L);

			//���ػ���
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_ANDROID_USER,0L,NULL,0L);

			//���ص���
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_GAME_PROPERTY,0L,NULL,0L);

			//������Ϣ
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);

			//���ر���ʱ��������Ϣ
			//m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_MATCH_CONFIG,0L,NULL,0L);

			//���ر�������������Ϣ
			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_MATCH_REWARD_CONFIG,0L,NULL,0L);

			return true;
		}
	}

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
	//����ʱ��
	if ((dwTimerID>=IDI_MAIN_MODULE_START)&&(dwTimerID<=IDI_MAIN_MODULE_FINISH))
	{
		//ʱ�䴦��
		switch (dwTimerID)
		{
		case IDI_LOAD_ANDROID_USER:		//���ػ���
			{
				//���ػ���
				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_ANDROID_USER,0L,NULL,0L);

				return true;
			}
		case IDI_REPORT_SERVER_INFO:	//������Ϣ
			{
				//��������
				CMD_CS_C_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//���ñ���
				ServerOnLine.dwOnLineCount=m_ServerUserManager.GetUserItemCount();

				//��������
				m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine));

				return true;
			}
		case IDI_CONNECT_CORRESPOND:	//����Э��
			{
				//��������
				tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
				m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//������ʾ
				TCHAR szString[512]=TEXT("");
				_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//��ʾ��Ϣ
				CTraceService::TraceString(szString,TraceLevel_Normal);

				return true;
			}
		case IDI_GAME_SERVICE_PULSE:	//����ά��
			{
				return true;
			}
		case IDI_DISTRIBUTE_ANDROID:	//�������
			{
				TCHAR szString9[256]=TEXT("");

				//��������
				if (m_AndroidUserManager.GetAndroidCount()>0 && m_pGameServiceOption->wServerType!=GAME_GENRE_MATCH)
				{
					//�������壻��̬���롢�������桢����ռλ��������
					bool bAllowDynamicJoin=CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule);
					bool bAllowAndroidAttend=CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule);
					bool bAllowAndroidSimulate=CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule);
					bool bAllowAvertCheatMode=(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR));

					//add
					int nTableCount=m_pGameServiceOption->wTableCount;//*2/3;

					//ģ�⴦��
					if (bAllowAndroidSimulate==true && bAllowAvertCheatMode==false)
					{
						//����״̬
						tagAndroidUserInfo AndroidSimulate; 
						m_AndroidUserManager.GetAndroidUserInfo(AndroidSimulate,ANDROID_SIMULATE);

						//������������
						UINT nSitTable=-1;

						//��������
						if (AndroidSimulate.wFreeUserCount>0)
						{
							for (WORD i=0;i<8;i++)
							{
								//ѡȡҪ����������
								UINT nRandProb = rand()%3; //������֮���ĸ�����������
								if (nRandProb != 0)
								{
									for (UINT m=0; m<sizeof(m_PrecedenceTableArray)/m_PrecedenceTableArray[0]; m++)
									{
										//����״��
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

								//������ѡȡ����
								nRandProb = rand()%5; //��5��֮һ�ĸ����������һ��������
								if ((nSitTable == -1) && (nRandProb != 0) && (m_RandPartNum != 1))
								{
									//����������
									UINT nSectionNum=m_RandPartNum;
	
									//ÿ����������
									UINT nTableCntPerSection=nTableCount/nSectionNum;
	
									//��С�ɲ������������
									UINT nMinTable=nTableCount;
	
									//�������������;
									UINT nInsSection=1;

									for (UINT j=0; j < nTableCount; j++)
									{
										if ((m_TableFrameArray[j]->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) 
										{
											m_TableFrameArray[j]->SetDynamicJoinAndriod(false);
											continue;
										}
	
										//����״��
										tagTableUserInfo TableUserInfoTemp;
										WORD wUserSitCount=m_TableFrameArray[j]->GetTableUserInfo(TableUserInfoTemp);
	
										//add
										if (TableUserInfoTemp.wTableAndroidCount>=m_TableFrameArray[j]->GetChairCount() * 2/3)
										{
											m_TableFrameArray[j]->SetDynamicJoinAndriod(false);
											continue;
										}
	
										//���ÿɲ�������˱�־λ
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
	
										//��ȡ������ɲ��������������Ŀ
										UINT nJoinAndroidTableCnt=0;
										for (UINT l=(nTableCntPerSection * (nInsSection-1)); l < nTableCntPerSection * nInsSection; l++)
										{
											if (m_TableFrameArray[l]->GetDynamicJoinAndriod() == true)
											{
												nJoinAndroidTableCnt++;
											}
										}
	
										//�����=rand()%ÿ�ݴ�С+1;
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
									//�������
									nSitTable=rand()%(__max(nTableCount,1));

									//��ȡ����
									CTableFrame * pTableFrame=m_TableFrameArray[nSitTable];
									if ((pTableFrame->IsGameStarted()==true)&&(bAllowDynamicJoin==false)) continue;

									//����״��
									tagTableUserInfo TableUserInfo;
									WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

									//add
									if (TableUserInfo.wTableAndroidCount>=pTableFrame->GetChairCount()/2) continue;//������һ������
								}

								//��ȡ����
								CTableFrame * pTableFrame=NULL;

								//����״��
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

								//�����ж�
								if (AndroidSimulate.wFreeUserCount>=TableUserInfo.wMinUserCount)
								{
									//_sntprintf(szString9,CountArray(szString9),TEXT("21*****"));
									//CTraceService::TraceString(szString9,TraceLevel_Normal);

									//��������
									WORD wHandleCount=0;
									WORD wWantAndroidCount=TableUserInfo.wMinUserCount;

									//���ݵ���
									if (m_pGameServiceAttrib->wChairCount>TableUserInfo.wMinUserCount)
									{
										WORD wChairCount=m_pGameServiceAttrib->wChairCount-2;//��������
										WORD wFreeUserCount=AndroidSimulate.wFreeUserCount;
										WORD wOffUserCount=__min(wChairCount,wFreeUserCount)-TableUserInfo.wMinUserCount;
										wWantAndroidCount+=(wOffUserCount > 0) ? (rand()%wOffUserCount) : 0;
									}

									//���´���
									for (WORD j=0;j<AndroidSimulate.wFreeUserCount;j++)
									{
										//��������
										WORD wChairID=pTableFrame->GetRandNullChairID();

										//��Ч����
										//ASSERT(wChairID!=INVALID_CHAIR);
										if (wChairID==INVALID_CHAIR) continue;

										//�û�����
										IAndroidUserItem * pIAndroidUserItem=AndroidSimulate.pIAndroidUserFree[j];

										if (pIAndroidUserItem!=NULL && pIAndroidUserItem->GetMeUserItem()->GetMemberOrder()==5) continue;

										if (pTableFrame->PerformSitDownAction(wChairID,pIAndroidUserItem->GetMeUserItem())==true)
										{
											//_sntprintf(szString9,CountArray(szString9),TEXT("14*****"));
											//CTraceService::TraceString(szString9,TraceLevel_Normal);
											//���ñ���
											wHandleCount++;

											//����ж�
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

					//��������

					//�ķ�֮һ�ĸ�����������
					UINT nRandStandUp=rand()%4;

					if (nRandStandUp != 0)
					{
						WORD wStandUpCount=0;
						WORD wRandCount=(rand()%3+1);
						INT_PTR nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));

						for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
						{
							//��ȡ����
							INT_PTR nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));
							INT_PTR nTableIndex=nIndex;
							CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
							//if (pTableFrame->IsGameStarted()==true) continue;//��Ϸ�Ѿ���ʼ

							//����״��
							tagTableUserInfo TableUserInfo;
							WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

							//�û�����
							if (TableUserInfo.wTableAndroidCount==0) continue;
							if ((TableUserInfo.wTableUserCount>0)&&(bAllowAndroidAttend==true)) continue;

							//��������
							for (WORD j=0;j<pTableFrame->GetChairCount();j++)
							{
								//��ȡ�û�
								IServerUserItem * pIServerUserItem=pTableFrame->GetTableUserItem(j);
								if (pIServerUserItem==NULL) continue;

								//�û�����
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
		case IDI_DBCORRESPOND_NOTIFY: //���涨ʱ����
			{
				if(m_pIDBCorrespondManager) m_pIDBCorrespondManager->OnTimerNotify();
				return true;
			}
		case IDI_LOAD_SYSTEM_MESSAGE: //ϵͳ��Ϣ
			{
				//�����Ϣ����
				ClearSystemMessageData();

				//������Ϣ
				m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);
				return true;
			}
		case IDI_LOAD_SENSITIVE_WORD:	//�������д�
			{
				//Ͷ������
				m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_SENSITIVE_WORDS,0,NULL,0);				
				return true;
			}
		case IDI_SEND_SYSTEM_MESSAGE: //ϵͳ��Ϣ
			{
				//�����ж�
				if(m_SystemMessageList.GetCount()==0) return true;

				//ʱЧ�ж�
				DWORD dwCurrTime = (DWORD)time(NULL);
				POSITION pos = m_SystemMessageList.GetHeadPosition();
				while(pos != NULL)
				{
					POSITION tempPos = pos;
					tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);
					if(pTagSystemMessage->dwLastTime+pTagSystemMessage->SystemMessage.dwTimeRate < dwCurrTime)
					{
						//��������
						pTagSystemMessage->dwLastTime=dwCurrTime;

						//������Ϣ
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

						//������Ϣ
						WORD wSendSize = sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+CountStringBuffer(SendMessage.szSystemMessage);
						SendSystemMessage(&SendMessage,wSendSize);
					}
				}


				return true;
			}
		case IDI_DISTRIBUTE_USER: //�����û�
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
					TEXT("��ϲ��%s���ڳ���������Ӯ��%ld��󽱡�"),g_pszFakeName[wFNNum],wRewardNum);
				//CTraceService::TraceString(stFakeAnnounce.szClownFakeAnnounceMent,TraceLevel_Normal);

				m_pITCPNetworkEngine->SendDataBatch(MDM_GR_MANAGE,SUB_GR_CLOWN_FAKEANNOUNCEMENT,(VOID *)&stFakeAnnounce,sizeof(CMD_GR_CLOWNFAKEANNOUNCE),BG_COMPUTER);	
				
			}
			break;
		case IDI_READ_CONFIG:
			{
				//��ȡ����
				TCHAR szPath[MAX_PATH]=TEXT("");
				TCHAR szConfigFileName[MAX_PATH] = TEXT("");
				GetCurrentDirectory(sizeof(szPath),szPath);
				_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\GameServerTimerInterval.ini"),szPath);

				//��ȡ����
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
		//		//��ȡ����
		//		TCHAR szPath[MAX_PATH]=TEXT("");
		//		TCHAR szConfigFileName[MAX_PATH] = TEXT("");
		//		GetCurrentDirectory(sizeof(szPath),szPath);
		//		_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\GameServerMatchConfig.ini"),szPath);

		//		//��ȡ����
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
		//			//���ر�������
		//			m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_MATCH_CONFIG,0L,NULL,0L);
		//		}
		//		tagMatchConfigItem sMatchConfigItem;
		//		GetNextMatchInfo(sMatchConfigItem);
		//		int nMatchStartTimeHour = sMatchConfigItem.nStartTime/100;
		//		int nMatchStartTimeMinute = sMatchConfigItem.nStartTime%100;
		//		if (nMatchStartTimeHour==st.wHour&&nMatchStartTimeMinute-st.wMinute==m_nMessageRemindTime1)
		//		{
		//			//������ʾ��Ϣ
		//		}
		//		else if (nMatchStartTimeHour==st.wHour&&nMatchStartTimeMinute-st.wMinute==m_nMessageRemindTime2)
		//		{
		//			//������ʾ��Ϣ
		//		}
		//		if (nMatchStartTimeHour==st.wHour&&nMatchStartTimeMinute-st.wMinute==m_nMessageRemindTime3)
		//		{
		//			//������ʾ��Ϣ
		//			m_pITimerEngine->SetTimer(IDI_MATCH_BEGIN_REMINDER,1000,(DWORD)(-1),NULL);

		//		}
		//		
		//	}
		//	break;
		//case IDI_MATCH_BEGIN_REMINDER:
		//	{
		//		//������ʾ��Ϣ
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

	//����ʱ��
	if ((dwTimerID>=IDI_REBOT_MODULE_START)&&(dwTimerID<=IDI_REBOT_MODULE_FINISH))
	{
		//ʱ�䴦��
		m_AndroidUserManager.OnEventTimerPulse(dwTimerID,wBindParam);

		return true;
	}

	//������ʱ��
	if((dwTimerID>=IDI_MATCH_MODULE_START)&&(dwTimerID<IDI_MATCH_MODULE_FINISH))
	{
		if(m_pIGameMatchServiceManager!=NULL) m_pIGameMatchServiceManager->OnEventTimer(dwTimerID,wBindParam);
		return true;
	}

	//����ʱ��
	if ((dwTimerID>=IDI_TABLE_MODULE_START)&&(dwTimerID<=IDI_TABLE_MODULE_FINISH))
	{
		//���Ӻ���
		DWORD dwTableTimerID=dwTimerID-IDI_TABLE_MODULE_START;
		WORD wTableID=(WORD)(dwTableTimerID/TIME_TABLE_MODULE_RANGE);

		//ʱ��Ч��
		if (wTableID>=(WORD)m_TableFrameArray.GetCount()) 
		{
			ASSERT(FALSE);
			return false;
		}

		//ʱ��֪ͨ
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		return pTableFrame->OnEventTimer(dwTableTimerID%TIME_TABLE_MODULE_RANGE,wBindParam);
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GR_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GR_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GAME_PARAMETER:			//��Ϸ����
		{
			return OnDBGameParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GAME_COLUMN_INFO:		//�б���Ϣ
		{
			return OnDBGameColumnInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GR_GAME_ANDROID_INFO:		//������Ϣ
		{
			return OnDBGameAndroidInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GAME_PROPERTY_INFO:		//������Ϣ
		{
			return OnDBGamePropertyInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_INFO:		//������Ϣ
		{
			return OnDBUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_SUCCESS:	//���гɹ�
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
			//	TEXT("��������"), TEXT("�м�����"), TEXT("�߼�����"),
			//};

			//if (pWinPearl->nPearlType < _countof(pPearlType))
			//{
			//	_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff����#ff0000%s#ffffff���˵Ĳ���#ff0000%s#ffffffһ�š�", pIServerUserItem->GetNickName(), m_pGameServiceOption->szServerName, pPearlType[pWinPearl->nPearlType]);

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
	case DBO_GR_USER_INSURE_FAILURE:	//����ʧ��
		{
			return OnDBUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_USER_INFO:  //�û���Ϣ
		{
			return OnDBUserInsureUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_PROPERTY_SUCCESS:		//���߳ɹ�
		{
			return OnDBPropertySuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GR_SYSTEM_MESSAGE_RESULT:  //ϵͳ��Ϣ
		{
			return OnDBSystemMessage(dwContextID,pData,wDataSize);
		}
	case DBO_GR_SENSITIVE_WORDS:	//�������д�
		{
			return OnDBSensitiveWords(dwContextID,pData,wDataSize);
		}
	case DBR_GR_CHECKSTOCK_SUCCESS:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(tagDBR_GR_CheckStockSuccess));
			if (wDataSize!=sizeof(tagDBR_GR_CheckStockSuccess)) return false;

			//��Ϣ����
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
			szMessage.Format(TEXT("��ϲ���[%s]��[%s]���䣬����ݵ����%d�֡�"),pPresentScore->szNickName,pPresentScore->szServerName,pPresentScore->nScore);
			//������Ϣ
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

			//��ȡ�û�
			WORD wBindIndex=LOWORD(dwContextID);
			IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

			if (pCheckTrumpet->lReturnValue == 1)
			{
				//������Ϣ
				SendPropertyFailure(pIServerUserItem,TEXT("�Բ������Ļ�Ա�ȼ����㣬���ܷ��ʹ����ȡ�"), 0L, pCheckTrumpet->cbRequestArea);
				return true;
			}
			if (pCheckTrumpet->lReturnValue == 2)
			{
				//������Ϣ
				SendPropertyFailure(pIServerUserItem,TEXT("�Բ������Ĵ������������㣬���ܷ��͡�"), 0L, pCheckTrumpet->cbRequestArea);
				return true;
			}

			////��������
			//BYTE cbIndex=(pCheckTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?2:3;

			//����ṹ
			CMD_GR_S_SendTrumpet  SendTrumpet;
			SendTrumpet.dwSendUserID=pCheckTrumpet->dwUserID;
			SendTrumpet.wPropertyIndex=pCheckTrumpet->wPropertyIndex;
			SendTrumpet.TrumpetColor=pCheckTrumpet->TrumpetColor;
			ZeroMemory(SendTrumpet.szTrumpetContent,sizeof(SendTrumpet.szTrumpetContent));
			CopyMemory(SendTrumpet.szSendNickName,pIServerUserItem->GetNickName(),sizeof(SendTrumpet.szSendNickName));

			//�ַ�����
			SensitiveWordFilter(pCheckTrumpet->szTrumpetContent,SendTrumpet.szTrumpetContent,CountArray(SendTrumpet.szTrumpetContent));

			////����ת��
			//if(cbIndex==3)
			{
				//�㲥����
				WORD wUserIndex=0;
				CMD_CS_S_SendTrumpet * pSendTrumpet = (CMD_CS_S_SendTrumpet *)&SendTrumpet;
				if(m_pITCPSocketService)
				{
					m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIP_TRUMPET,pSendTrumpet,sizeof(CMD_CS_S_SendTrumpet));
				}
			}

			////��Ϸת��
			//if(cbIndex==2)
			//{
			//	//��������
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

			////��������
			//WORD wItemCount=wDataSize/sizeof(DBO_Load_Match_Config_Item);
			//DBO_Load_Match_Config_Item * pGameItem=(DBO_Load_Match_Config_Item *)pData;
			//m_MatchConfigItemList.clear();
			////��������
			//for (WORD i=0;i<wItemCount;i++)
			//{
			//	//��������
			//	tagMatchConfigItem MatchConfigItem;
			//	ZeroMemory(&MatchConfigItem,sizeof(MatchConfigItem));

			//	//��������
			//	MatchConfigItem.nStartTime = pGameItem->nStartTime;
			//	MatchConfigItem.nEndTime = pGameItem->nEndTime;
			//	MatchConfigItem.nApplyCost = pGameItem->nApplyCost;
			//	MatchConfigItem.nGameTime = pGameItem->nGameTime;
			//	MatchConfigItem.nMatchNum = pGameItem->nMatchNum;
			//	MatchConfigItem.nMatchUserCount = pGameItem->nMatchUserCount;
			//	MatchConfigItem.nMatchType = pGameItem->nMatchType;
			//	//�����б�
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

			//��������
			WORD wItemCount=wDataSize/sizeof(DBO_Load_Match_Reward_Config_Item);
			DBO_Load_Match_Reward_Config_Item * pGameItem=(DBO_Load_Match_Reward_Config_Item *)pData;
			m_MatchRewardConfigItemList.clear();
			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				//��������
				tagMatchRewardConfigItem MatchConfigItem;
				ZeroMemory(&MatchConfigItem,sizeof(MatchConfigItem));

				//��������
				MatchConfigItem.nRankStart = pGameItem->nRankStart;
				MatchConfigItem.nRankEnd = pGameItem->nRankEnd;
				MatchConfigItem.nMachType = pGameItem->nMachType;
				MatchConfigItem.nShareType = pGameItem->nShareType;

				for (int i=0;i<10;++i)
				{
					MatchConfigItem.nReward[i] = pGameItem->nReward[i];
				}
				//�����б�
				m_MatchRewardConfigItemList.push_back(MatchConfigItem);
			}
			return true;
		}
	case DBO_GR_MATCH_GET_PRIZE:
		{
			ASSERT(wDataSize%sizeof(DBO_GR_Match_Get_Prize)==0);
			if (wDataSize%sizeof(DBO_GR_Match_Get_Prize)!=0) return false;
			//��������
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
				sprintf(szText,"��ȡ�ɹ������%d��ҡ�",pDBMatchResult->nPriseCount);
				lstrcpyn(stMatchGetMatchPrizeRes.szDescription,szText,sizeof(stMatchGetMatchPrizeRes.szDescription));
			}
			else
			{
				char szText[128];
				sprintf(szText,"�����Ѿ���ȡ",pDBMatchResult->nPriseCount);
				lstrcpyn(stMatchGetMatchPrizeRes.szDescription,szText,sizeof(stMatchGetMatchPrizeRes.szDescription));
			}
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GR_MATCH_GET_MATCH_PRIZE_RES, &stMatchGetMatchPrizeRes, sizeof(CMD_GPO_MatchGetMatchPriseRes));
			return true;
		}
	}

	//�����¼�
	if(wRequestID>=DBO_GR_MATCH_EVENT_START && wRequestID<=DBO_GR_MATCH_EVENT_END)
	{
		//����Ч��
		if(m_pIGameMatchServiceManager==NULL) return false;

		tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
		IServerUserItem *pIServerUserItem=pBindParameter!=NULL?pBindParameter->pIServerUserItem:NULL;

		//�����ж�
		if ((pBindParameter->pIServerUserItem==NULL)||(pBindParameter->dwSocketID!=dwContextID))
		{
			//�������
			ASSERT(FALSE);
			return true;
		}

		return m_pIGameMatchServiceManager->OnEventDataBase(wRequestID,pIServerUserItem,pData,wDataSize);
	}

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//���ñ���
		m_bCollectUser=false;

		//ɾ��ʱ��
		m_pITimerEngine->KillTimer(IDI_REPORT_SERVER_INFO);

		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);
		}

		return true;
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (nErrorCode!=0)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("Э������������ʧ�� [ %ld ]��%ld �����������"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		CTraceService::TraceString(TEXT("���ڷ�����Ϸ����ע����Ϣ..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterServer RegisterServer;
		ZeroMemory(&RegisterServer,sizeof(RegisterServer));

		//����˿�
		CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
		RegisterServer.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();

		//��������
		RegisterServer.wKindID=m_pGameServiceOption->wKindID;
		RegisterServer.wNodeID=m_pGameServiceOption->wNodeID;
		RegisterServer.wSortID=m_pGameServiceOption->wSortID;
		RegisterServer.wServerID=m_pGameServiceOption->wServerID;
		RegisterServer.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		RegisterServer.dwFullCount=m_pGameServiceOption->wMaxPlayer-RESERVE_USER_COUNT;
		RegisterServer.lMinEnterScore=m_pGameServiceOption->lMinEnterScore;
		lstrcpyn(RegisterServer.szServerName,m_pGameServiceOption->szServerName,CountArray(RegisterServer.szServerName));
		lstrcpyn(RegisterServer.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterServer.szServerAddr));

		//��������
		ASSERT(m_pITCPSocketService!=NULL);
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_SERVER,&RegisterServer,sizeof(RegisterServer));

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_REPORT_SERVER_INFO,TIME_REPORT_SERVER_INFO*1000L,TIMES_INFINITY,0);

		return true;
	}

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//ע�����
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//������Ϣ
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_USER_COLLECT:	//�û�����
			{
				return OnTCPSocketMainUserCollect(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //�������
			{
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//���ñ���
	if (pBindParameter!=NULL)
	{
		pBindParameter->dwSocketID=dwSocketID;
		pBindParameter->dwClientAddr=dwClientAddr;
		pBindParameter->dwActiveTime=(DWORD)time(NULL);

		return true;
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=pBindParameter->pIServerUserItem;

	//�û�����
	if (pIServerUserItem!=NULL)
	{
		//��������
		WORD wTableID=pIServerUserItem->GetTableID();

		//���ߴ���
		if (wTableID!=INVALID_TABLE)
		{
			//�����
			pIServerUserItem->DetachBindStatus();

			//����֪ͨ
			ASSERT(wTableID<m_pGameServiceOption->wTableCount);
			m_TableFrameArray[wTableID]->OnEventUserOffLine(pIServerUserItem);
		}
		else
		{
			pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
		}
	}

	//�����Ϣ
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));

	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	bool bRet=false;
	switch (Command.wMainCmdID)
	{
	case MDM_GR_USER:		//�û�����
		{
			bRet=OnTCPNetworkMainUser(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_LOGON:		//��¼����
		{
			bRet=OnTCPNetworkMainLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GF_GAME:		//��Ϸ����
		{
			bRet=OnTCPNetworkMainGame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GF_FRAME:		//�������
		{
			bRet=OnTCPNetworkMainFrame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_INSURE:		//��������
		{
			bRet=OnTCPNetworkMainInsure(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_MANAGE:		//��������
		{
			bRet=OnTCPNetworkMainManage(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	case MDM_GR_MATCH:		//��������
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

//������Ϣ
bool CAttemperEngineSink::SendRoomMessage(LPCTSTR lpszMessage, WORD wType)
{
	//��������
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//��������
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//��������
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//��������
	m_AndroidUserManager.SendDataToClient(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_COMPUTER);

	return true;
}

//��Ϸ��Ϣ
bool CAttemperEngineSink::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//��������
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//��������
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//��������
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//��������
	m_AndroidUserManager.SendDataToClient(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_COMPUTER);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//��������
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//��������
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//��������
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//��������
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//��������
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);

			//�رմ���
			if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0) m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
		}
		else
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//��Ϸ��Ϣ
bool CAttemperEngineSink::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	if ((pIServerUserItem->GetBindIndex()!=INVALID_WORD)&&(pIServerUserItem->IsClientReady()==true))
	{
		//��������
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//��������
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//��������
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//��������
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//��������
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);

			//�رմ���
			if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0) m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
		}
		else
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//������Ϣ
bool CAttemperEngineSink::SendRoomMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wType, bool bAndroid)
{
	//��������
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//��������
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//��������
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//��������
	if (bAndroid)
	{
		//�����û�
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	}
	else
	{
		//�����û�
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	if ((cbSendMask&BG_COMPUTER)!=0)
	{
		m_AndroidUserManager.SendDataToClient(wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	//�û�����
	m_pITCPNetworkEngine->SendDataBatch(wMainCmdID,wSubCmdID,pData,wDataSize,cbSendMask);

	return true;
}

//��������
bool CAttemperEngineSink::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	if (LOWORD(dwSocketID)!=INVALID_WORD)
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			//�����û�
			m_AndroidUserManager.SendDataToClient(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else 
		{
			//�����û�
			m_pITCPNetworkEngine->SendData(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else
		{
			//�����û�
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}

		return true;
	}

	return false;
}

//�û�����
bool CAttemperEngineSink::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_UserScore UserScore;
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	//��������
	UserScore.dwUserID=pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	UserScore.UserScore.dwUserMedal=pUserInfo->dwUserMedal;
	UserScore.UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	//�������
	UserScore.UserScore.lGrade=pUserInfo->lGrade;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;

	//�������
	UserScore.UserScore.lScore=pUserInfo->lScore;
	UserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	UserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//��������
	SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));

	//��������
	CMD_GR_MobileUserScore MobileUserScore;

	//��������
	MobileUserScore.dwUserID=pUserInfo->dwUserID;
	MobileUserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	MobileUserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	MobileUserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	MobileUserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	MobileUserScore.UserScore.dwExperience=pUserInfo->dwExperience;

	//�������
	MobileUserScore.UserScore.lScore=pUserInfo->lScore;
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//��������
	SendDataBatchToMobileUser(pIServerUserItem->GetTableID(),MDM_GR_USER,SUB_GR_USER_SCORE,&MobileUserScore,sizeof(MobileUserScore));

	//��ʱд��
	if ((CServerRule::IsImmediateWriteScore(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->IsVariation()==true))
	{
		//��������
		DBR_GR_WriteGameScore WriteGameScore;
		ZeroMemory(&WriteGameScore,sizeof(WriteGameScore));

		//�û���Ϣ
		WriteGameScore.dwUserID=pIServerUserItem->GetUserID();
		WriteGameScore.dwDBQuestID=pIServerUserItem->GetDBQuestID();
		WriteGameScore.dwClientAddr=pIServerUserItem->GetClientAddr();
		WriteGameScore.dwInoutIndex=pIServerUserItem->GetInoutIndex();

		//��ȡ����
		pIServerUserItem->DistillVariation(WriteGameScore.VariationInfo);

		//Ͷ������
		m_pIDBCorrespondManager->PostDataBaseRequest(WriteGameScore.dwUserID,DBR_GR_WRITE_GAME_SCORE,0L,&WriteGameScore,sizeof(WriteGameScore), TRUE);
	}

	//֪ͨ����
	if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		m_TableFrameArray[pIServerUserItem->GetTableID()]->OnUserScroeNotify(pIServerUserItem->GetChairID(),pIServerUserItem,cbReason);
	}
	return true;
}

//�û�״̬
bool CAttemperEngineSink::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID)
{


	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_UserStatus UserStatus;
	ZeroMemory(&UserStatus,sizeof(UserStatus));

	//��������
	UserStatus.dwUserID=pIServerUserItem->GetUserID();
	UserStatus.UserStatus.wTableID=pIServerUserItem->GetTableID();
	UserStatus.UserStatus.wChairID=pIServerUserItem->GetChairID();
	UserStatus.UserStatus.cbUserStatus=pIServerUserItem->GetUserStatus();

	//�޸���Ϣ
	if(pIServerUserItem->GetUserStatus()==US_SIT && pIServerUserItem->IsMobileUser())
	{
		//�������
		WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
		WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();

		if(pIServerUserItem->GetTableID() > (wTagerDeskPos+wTagerDeskCount-1))
		{
			//�������
			WORD wNewDeskPos = (pIServerUserItem->GetTableID()/wTagerDeskCount)*wTagerDeskCount;
			WORD wMaxDeskPos = m_pGameServiceOption->wTableCount-wTagerDeskCount;

			//����Ч��
			if(wNewDeskPos > wMaxDeskPos) wNewDeskPos = wMaxDeskPos;

			//�޸���Ϣ
			pIServerUserItem->SetMobileUserDeskPos(wNewDeskPos);
		}
	}

	//��������
	SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	if(pIServerUserItem->GetUserStatus()==US_SIT)
		SendDataBatchToMobileUser(pIServerUserItem->GetTableID(),MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	else
		SendDataBatchToMobileUser(wOldTableID,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));

	//�뿪�ж�
	if (pIServerUserItem->GetUserStatus()==US_NULL)
	{
		//��ȡ��
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

		//�������
		if (pBindParameter!=NULL)
		{
			//�󶨴���
			if (pBindParameter->pIServerUserItem==pIServerUserItem)
			{
				pBindParameter->pIServerUserItem=NULL;
			}

			//�ж�����
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

		//�뿪����
		OnEventUserLogout(pIServerUserItem,0L);
	}
	return true;
}

//�û�Ȩ��
bool CAttemperEngineSink::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	DBR_GR_ManageUserRight ManageUserRight= {0};
	ManageUserRight.dwUserID = pUserInfo->dwUserID;
	ManageUserRight.dwAddRight = dwAddRight;
	ManageUserRight.dwRemoveRight = dwRemoveRight;
	ManageUserRight.bGameRight=bGameRight;

	//��������
	m_pIDBCorrespondManager->PostDataBaseRequest(ManageUserRight.dwUserID,DBR_GR_MANAGE_USER_RIGHT,0,&ManageUserRight,sizeof(ManageUserRight));

	return true;
}

//�û���־ ����:wRequestID ������Ϊ��չ�ӿ�, ��ʱû���ҵ����Է��ù�����Ϣ�ŵ��ļ�û��ʵ��
bool CAttemperEngineSink::OnEventUserItemLog(IServerUserItem *pIServerUserItem, WORD wRequestID, VOID * pData, DWORD dwDataSize)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return true;

	//С�������־
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_USERLOG_CLOWN_OPERATION,0L,pData,dwDataSize);

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	WORD wBindIndex=LOWORD(dwContextID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	DBO_GR_LogonSuccess * pDBOLogonSuccess=(DBO_GR_LogonSuccess *)pData;

	//�����ж�
	if ((pBindParameter->pIServerUserItem!=NULL)||(pBindParameter->dwSocketID!=dwContextID))
	{
		//�������
		ASSERT(FALSE);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_NORMAL);

		return true;
	}

	//��������
	bool bAndroidUser=(wBindIndex>=INDEX_ANDROID);
	bool bMobileUser=(pBindParameter->cbClientKind==CLIENT_KIND_MOBILE);

	//�����ж�
	DWORD dwKickTime;
	if(m_KickUserItemMap.Lookup(pDBOLogonSuccess->dwUserID,dwKickTime)==TRUE)
	{
		//ʱЧ�ж�
		DWORD dwCurrTime = (DWORD)time(NULL);
		if(dwKickTime+VALID_TIME_KICK_BY_MANAGER > dwCurrTime)
		{
			//����ʧ��
			SendLogonFailure(TEXT("���ѱ�����Ա�������,1Сʱ֮�ڲ��ܽ��룡"),0,pBindParameter->dwSocketID);

			//�������
			PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

			return true;
		}
		else
		{
			//�Ƴ����
			m_KickUserItemMap.RemoveKey(pDBOLogonSuccess->dwUserID);
		}
	}

	//�л��ж�
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		//�л��û�
		SwitchUserItemConnect(pIServerUserItem,pDBOLogonSuccess->szMachineID,wBindIndex,pDBOLogonSuccess->cbDeviceType,pDBOLogonSuccess->wBehaviorFlags,pDBOLogonSuccess->wPageTableCount);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_USER_IMPACT);

		return true;
	}

	//ά���ж�
	if ((CServerRule::IsForfendRoomEnter(m_pGameServiceOption->dwServerRule)==true)&&(pDBOLogonSuccess->cbMasterOrder==0))
	{
		//����ʧ��
		SendLogonFailure(TEXT("��Ǹ������ϵͳά����ԭ�򣬵�ǰ��Ϸ�����ֹ�û����룡"),0,pBindParameter->dwSocketID);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SYSTEM);

		return true;
	}

	//���һ���
	if (bAndroidUser==true)
	{
		//���һ���
		DWORD dwUserID=pDBOLogonSuccess->dwUserID;
		IAndroidUserItem * pIAndroidUserItem=m_AndroidUserManager.SearchAndroidUserItem(dwUserID,dwContextID);

		//�޸Ļ���
		if (pIAndroidUserItem!=NULL)
		{
			//��ȡ����
			tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();

			//��������
			if ((pAndroidParameter->lMinTakeScore!=0L)&&(pAndroidParameter->lMaxTakeScore!=0L))
			{
				//��������
				SCORE lMinTakeScore=(SCORE)pAndroidParameter->lMinTakeScore;
				SCORE lMaxTakeScore=(SCORE)__max(pAndroidParameter->lMaxTakeScore,pAndroidParameter->lMinTakeScore);

				//��������
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

	//��ͷ���
	//if (m_pGameServiceOption->wKindID==2008)
	//{
	//	if ((m_pGameServiceOption->lMinEnterScore!=0L)&&(pDBOLogonSuccess->lScore<m_pGameServiceOption->lMinEnterScore)&&(pDBOLogonSuccess->nTreasureMapCount<=0))
	//	{
	//		TCHAR szMsg[128]=TEXT("");
	//		_sntprintf(szMsg,CountArray(szMsg), TEXT("����ǰ�Ľ�Ҳ���%I64d������������������ս��ȥ���ַ��ಶ������������ս��"), m_pGameServiceOption->lMinEnterScore);
	//		SendLogonFailure(szMsg,0,pBindParameter->dwSocketID);
	//		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);
	//		return true;
	//	}
	//}
	if ((m_pGameServiceOption->lMinEnterScore!=0L)&&(pDBOLogonSuccess->lScore<m_pGameServiceOption->lMinEnterScore))
	{
		//����ʧ��
		TCHAR szMsg[128]=TEXT("");
		_sntprintf(szMsg,CountArray(szMsg), TEXT("���Ľ�������ڵ�ǰ��Ϸ�������ͽ�����%I64d�����ܽ��뵱ǰ��Ϸ���䣡"), m_pGameServiceOption->lMinEnterScore);
		SendLogonFailure(szMsg,0,pBindParameter->dwSocketID);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//��߷���
	if ((m_pGameServiceOption->lMaxEnterScore!=0L)&&(pDBOLogonSuccess->lScore>m_pGameServiceOption->lMaxEnterScore))
	{
		//����ʧ��
		TCHAR szMsg[128]=TEXT("");
		_sntprintf(szMsg,CountArray(szMsg), TEXT("��Ǹ��������Ϸ�ɼ����ڵ�ǰ��Ϸ�������߽���ɼ�%I64d�����ܽ��뵱ǰ��Ϸ���䣡"), m_pGameServiceOption->lMaxEnterScore);
		SendLogonFailure(szMsg,0,pBindParameter->dwSocketID);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//��Ա�ж�
	if(m_pGameServiceOption->cbMinEnterMember != 0 && pDBOLogonSuccess->cbMemberOrder < m_pGameServiceOption->cbMinEnterMember &&(pDBOLogonSuccess->cbMasterOrder==0))
	{
		//����ʧ��
		SendLogonFailure(TEXT("��Ǹ�����Ļ�Ա������ڵ�ǰ��Ϸ�������ͽ����Ա���������ܽ��뵱ǰ��Ϸ���䣡"),0,pBindParameter->dwSocketID);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//��Ա�ж�
	if(m_pGameServiceOption->cbMaxEnterMember != 0 && pDBOLogonSuccess->cbMemberOrder > m_pGameServiceOption->cbMaxEnterMember &&(pDBOLogonSuccess->cbMasterOrder==0))
	{
		//����ʧ��
		SendLogonFailure(TEXT("��Ǹ�����Ļ�Ա������ڵ�ǰ��Ϸ�������߽����Ա���������ܽ��뵱ǰ��Ϸ���䣡"),0,pBindParameter->dwSocketID);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//�����ж�
	WORD wMaxPlayer=m_pGameServiceOption->wMaxPlayer;
	DWORD dwOnlineCount=m_ServerUserManager.GetUserItemCount();
	if ((pDBOLogonSuccess->cbMemberOrder==0)&&(pDBOLogonSuccess->cbMasterOrder==0)&&(dwOnlineCount>(DWORD)(wMaxPlayer-RESERVE_USER_COUNT)))
	{
		//����ʧ��
		SendLogonFailure(TEXT("��Ǹ�����ڴ˷����Ѿ���������ͨ��Ҳ��ܼ��������ˣ�"),0,pBindParameter->dwSocketID);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		return true;
	}

	//�û�����
	tagUserInfo UserInfo;
	tagUserInfoPlus UserInfoPlus;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&UserInfoPlus,sizeof(UserInfoPlus));

	//��������
	UserInfo.wFaceID=pDBOLogonSuccess->wFaceID;
	UserInfo.dwUserID=pDBOLogonSuccess->dwUserID;
	UserInfo.dwGameID=pDBOLogonSuccess->dwGameID;
	UserInfo.dwGroupID=pDBOLogonSuccess->dwGroupID;
	UserInfo.dwCustomID=pDBOLogonSuccess->dwCustomID;
	lstrcpyn(UserInfo.szNickName,pDBOLogonSuccess->szNickName,CountArray(UserInfo.szNickName));

	//�û�����
	UserInfo.cbGender=pDBOLogonSuccess->cbGender;
	UserInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
	UserInfo.cbMasterOrder=pDBOLogonSuccess->cbMasterOrder;
	lstrcpyn(UserInfo.szGroupName,pDBOLogonSuccess->szGroupName,CountArray(UserInfo.szGroupName));
	lstrcpyn(UserInfo.szUnderWrite,pDBOLogonSuccess->szUnderWrite,CountArray(UserInfo.szUnderWrite));

	//״̬����
	UserInfo.cbUserStatus=US_FREE;
	UserInfo.wTableID=INVALID_TABLE;
	UserInfo.wChairID=INVALID_CHAIR;

	//������Ϣ
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

	//��¼��Ϣ
	UserInfoPlus.dwLogonTime=(DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex=pDBOLogonSuccess->dwInoutIndex;

	//�û�Ȩ��
	UserInfoPlus.dwUserRight=pDBOLogonSuccess->dwUserRight;
	UserInfoPlus.dwMasterRight=pDBOLogonSuccess->dwMasterRight;

	//��������
	UserInfoPlus.bMobileUser=bMobileUser;
	UserInfoPlus.bAndroidUser=bAndroidUser;
	UserInfoPlus.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	lstrcpyn(UserInfoPlus.szPassword,pDBOLogonSuccess->szPassword,CountArray(UserInfoPlus.szPassword));

	//������Ϣ
	UserInfoPlus.wBindIndex=wBindIndex;
	UserInfoPlus.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(UserInfoPlus.szMachineID,pDBOLogonSuccess->szMachineID,CountArray(UserInfoPlus.szMachineID));

	//�����û�
	m_ServerUserManager.InsertUserItem(&pIServerUserItem,UserInfo,UserInfoPlus);

	//�����ж�
	if (pIServerUserItem==NULL)
	{
		//�������
		ASSERT(FALSE);

		//�������
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		//�Ͽ��û�
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

	//�����û�
	pBindParameter->pIServerUserItem=pIServerUserItem;

	pIServerUserItem->SetVipInfo(pDBOLogonSuccess->cbVIPLevel, pDBOLogonSuccess->dwVIPExp);

	//�޸Ĳ���
	if(pIServerUserItem->IsMobileUser())
	{
		SetMobileUserParameter(pIServerUserItem,pDBOLogonSuccess->cbDeviceType,pDBOLogonSuccess->wBehaviorFlags,pDBOLogonSuccess->wPageTableCount);
	}

	//��¼�¼�
	OnEventUserLogon(pIServerUserItem,false);

	//�����û�
	if (m_bCollectUser==true)
	{
		//��������
		CMD_CS_C_UserEnter UserEnter;
		ZeroMemory(&UserEnter,sizeof(UserEnter));

		//���ñ���
		UserEnter.dwUserID=pIServerUserItem->GetUserID();
		UserEnter.dwGameID=pIServerUserItem->GetGameID();
		lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

		//������Ϣ
		UserEnter.cbGender=pIServerUserItem->GetGender();
		UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
		UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();
		UserEnter.cbAndroid=pIServerUserItem->IsAndroidUser();

		//������Ϣ
		ASSERT(m_pITCPSocketService!=NULL);
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));
	}

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem!=NULL)) return true;

	//���ʹ���
	DBO_GR_LogonFailure * pLogonFailure=(DBO_GR_LogonFailure *)pData;
	SendLogonFailure(pLogonFailure->szDescribeString,pLogonFailure->lResultCode,dwContextID);

	//�Ͽ�����
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

//������Ϣ
bool CAttemperEngineSink::OnDBGameParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GR_GameParameter));
	if (wDataSize!=sizeof(DBO_GR_GameParameter)) return false;

	//��������
	DBO_GR_GameParameter * pGameParameter=(DBO_GR_GameParameter *)pData;

	//������Ϣ
	m_pGameParameter->wMedalRate=pGameParameter->wMedalRate;
	m_pGameParameter->wRevenueRate=pGameParameter->wRevenueRate;

	//�汾��Ϣ
	m_pGameParameter->dwClientVersion=pGameParameter->dwClientVersion;
	m_pGameParameter->dwServerVersion=pGameParameter->dwServerVersion;

	//�汾Ч��
	if (VERSION_EFFICACY==TRUE)
	{
		//�汾�ж�
		bool bVersionInvalid=false;
		if (m_pGameParameter->dwClientVersion!=m_pGameServiceAttrib->dwClientVersion) bVersionInvalid=true;
		if (m_pGameParameter->dwServerVersion!=m_pGameServiceAttrib->dwServerVersion) bVersionInvalid=true;

		//��ʾ��Ϣ
		if (bVersionInvalid==true)
		{
			CTraceService::TraceString(TEXT("ƽ̨���ݿ��������汾ע����Ϣ�뵱ǰ����汾��Ϣ��һ��"),TraceLevel_Warning);
		}
	}

	return true;
}

//�б���Ϣ
bool CAttemperEngineSink::OnDBGameColumnInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GR_GameColumnInfo * pGameColumnInfo=(DBO_GR_GameColumnInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameColumnInfo)-sizeof(pGameColumnInfo->ColumnItemInfo);

	//Ч�����
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0])))) return false;

	//���ݴ���
	if (pGameColumnInfo->cbColumnCount==0)
	{
		//Ĭ���б�
	}
	else
	{
		//��������
		m_DataConfigColumn.cbColumnCount=pGameColumnInfo->cbColumnCount;
		CopyMemory(m_DataConfigColumn.ColumnItem,pGameColumnInfo->ColumnItemInfo,pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]));
	}

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBGameAndroidInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GR_GameAndroidInfo * pGameAndroidInfo=(DBO_GR_GameAndroidInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameAndroidInfo)-sizeof(pGameAndroidInfo->AndroidParameter);

	//Ч�����
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0])))) return false;

	//���û���
	if (pGameAndroidInfo->lResultCode==DB_SUCCESS)
	{
		m_AndroidUserManager.SetAndroidStock(pGameAndroidInfo->AndroidParameter,pGameAndroidInfo->wAndroidCount);
	}

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBGamePropertyInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GR_GamePropertyInfo * pGamePropertyInfo=(DBO_GR_GamePropertyInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GamePropertyInfo)-sizeof(pGamePropertyInfo->PropertyInfo);

	//Ч�����
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0])))) return false;

	//��ȡ״̬
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	enServiceStatus ServiceStatus=pServiceUnits->GetServiceStatus();

	//���õ���
	if (pGamePropertyInfo->lResultCode==DB_SUCCESS)
	{
		//���ù���
		m_GamePropertyManager.SetGamePropertyInfo(pGamePropertyInfo->PropertyInfo,pGamePropertyInfo->cbPropertyCount);

		//��������
		m_DataConfigProperty.cbPropertyCount=pGamePropertyInfo->cbPropertyCount;
		CopyMemory(m_DataConfigProperty.PropertyInfo,pGamePropertyInfo->PropertyInfo,pGamePropertyInfo->cbPropertyCount*sizeof(pGamePropertyInfo->PropertyInfo[0]));
	}

	//�¼�֪ͨ
	if (ServiceStatus!=ServiceStatus_Service)
	{
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_SERVICE_CONFIG_RESULT,&ControlResult,sizeof(ControlResult));
	}

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//��������
	DBO_GR_UserInsureInfo * pUserInsureInfo=(DBO_GR_UserInsureInfo *)pData;

	//��������
	CMD_GR_S_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//��������
	UserInsureInfo.cbActivityGame=pUserInsureInfo->cbActivityGame;
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lUserScore+=pIServerUserItem->GetUserScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetTrusteeScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetFrozenedScore();
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GR_INSURE,SUB_GR_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBUserInsureTransferInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_TRANSFER_INFO,pData,wDataSize);

	return true;
}

//���гɹ�
bool CAttemperEngineSink::OnDBUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//��������
	DBO_GR_UserInsureSuccess * pUserInsureSuccess=(DBO_GR_UserInsureSuccess *)pData;

	//��������
	SCORE lFrozenedScore=pUserInsureSuccess->lFrozenedScore;
	SCORE lInsureRevenue=pUserInsureSuccess->lInsureRevenue;
	SCORE lVariationScore=pUserInsureSuccess->lVariationScore;
	SCORE lVariationInsure=pUserInsureSuccess->lVariationInsure;

	//�ⶳ����
	if ((lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(lFrozenedScore)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//���в���
	if (pIServerUserItem->ModifyUserInsure(lVariationScore,lVariationInsure,lInsureRevenue)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	CMD_GR_S_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//�������
	UserInsureSuccess.cbActivityGame=pUserInsureSuccess->cbActivityGame;
	UserInsureSuccess.lUserScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwContextID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//���ʹ���
	DBO_GR_UserInsureFailure * pUserInsureFailure=(DBO_GR_UserInsureFailure *)pData;
	SendInsureFailure(pIServerUserItem,pUserInsureFailure->szDescribeString,pUserInsureFailure->lResultCode,pUserInsureFailure->cbActivityGame);

	//�ⶳ����
	if ((pUserInsureFailure->lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(pUserInsureFailure->lFrozenedScore)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//��ȡ�û�
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//��������
	DBO_GR_UserTransferUserInfo * pTransferUserInfo=(DBO_GR_UserTransferUserInfo *)pData;

	//��������
	CMD_GR_S_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//�������
	UserTransferUserInfo.cbActivityGame=pTransferUserInfo->cbActivityGame;
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szNickName,pTransferUserInfo->szNickName,CountArray(UserTransferUserInfo.szNickName));

	//��������
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_TRANSFER_USER_INFO,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//���߳ɹ�
bool CAttemperEngineSink::OnDBPropertySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GR_S_PropertySuccess * pPropertySuccess=(DBO_GR_S_PropertySuccess *)pData;

	//��ȡ�û�
	IServerUserItem * pISourceUserItem=m_ServerUserManager.SearchUserItem(pPropertySuccess->dwSourceUserID);
	IServerUserItem * pITargetUserItem=m_ServerUserManager.SearchUserItem(pPropertySuccess->dwTargetUserID);

	//�����û�
	if (pISourceUserItem!=NULL)
	{
		//��������
		SCORE lFrozenedScore=pPropertySuccess->lFrozenedScore;

		//�ⶳ����
		if ((lFrozenedScore>0L)&&(pISourceUserItem->UnFrozenedUserScore(lFrozenedScore)==false))
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//��������
	if (pISourceUserItem!=NULL)
	{
		pISourceUserItem->ModifyUserProperty(0,pPropertySuccess->lSendLoveLiness);
	}

	//��������
	if(pITargetUserItem!=NULL)
	{
		pITargetUserItem->ModifyUserProperty(0,pPropertySuccess->lRecvLoveLiness);
	}

	//��������
	DWORD dwCurrentTime=(DWORD)time(NULL);
	tagUserProperty * pUserProperty = pITargetUserItem->GetUserProperty();

	//���ߴ���	
	switch(pPropertySuccess->wPropertyIndex)
	{
	case PROPERTY_ID_TWO_CARD:       //˫������
		{
			//ʹ���ж�
			if((pUserProperty->wPropertyUseMark&PT_USE_MARK_DOUBLE_SCORE)!=0)
			{
				//��������
				DWORD  dwValidTime=pUserProperty->PropertyInfo[0].wPropertyCount*pUserProperty->PropertyInfo[0].dwValidNum;
				if(pUserProperty->PropertyInfo[0].dwEffectTime+dwValidTime<dwCurrentTime)
				{
					pUserProperty->PropertyInfo[0].dwEffectTime=dwCurrentTime;
					pUserProperty->PropertyInfo[0].wPropertyCount=pPropertySuccess->wItemCount;
					pUserProperty->PropertyInfo[0].dwValidNum=VALID_TIME_DOUBLE_SCORE;
				}
				else
				{
					//��Ŀ�ۼ�
					pUserProperty->PropertyInfo[0].wPropertyCount+=pPropertySuccess->wItemCount;
				}
			}
			else
			{
				//������Ϣ
				pUserProperty->PropertyInfo[0].dwEffectTime=dwCurrentTime;
				pUserProperty->PropertyInfo[0].wPropertyCount=pPropertySuccess->wItemCount;
				pUserProperty->PropertyInfo[0].dwValidNum=VALID_TIME_DOUBLE_SCORE;
				pUserProperty->wPropertyUseMark |= PT_USE_MARK_DOUBLE_SCORE;
			}

			break;
		}
	case PROPERTY_ID_FOUR_CARD:      //�ı�����
		{
			//ʹ���ж�
			if((pUserProperty->wPropertyUseMark&PT_USE_MARK_FOURE_SCORE)!=0)
			{
				//��������
				DWORD  dwValidTime=pUserProperty->PropertyInfo[1].wPropertyCount*pUserProperty->PropertyInfo[1].dwValidNum;
				if(pUserProperty->PropertyInfo[1].dwEffectTime+dwValidTime<dwCurrentTime)
				{
					pUserProperty->PropertyInfo[1].dwEffectTime=dwCurrentTime;
					pUserProperty->PropertyInfo[1].wPropertyCount=pPropertySuccess->wItemCount;
					pUserProperty->PropertyInfo[1].dwValidNum=VALID_TIME_FOUR_SCORE;
				}
				else
				{
					//��Ŀ�ۼ�
					pUserProperty->PropertyInfo[1].wPropertyCount+=pPropertySuccess->wItemCount;
				}
			}
			else
			{
				//������Ϣ
				pUserProperty->PropertyInfo[1].dwEffectTime=dwCurrentTime;
				pUserProperty->PropertyInfo[1].wPropertyCount=pPropertySuccess->wItemCount;
				pUserProperty->PropertyInfo[1].dwValidNum=VALID_TIME_FOUR_SCORE;
				pUserProperty->wPropertyUseMark |= PT_USE_MARK_FOURE_SCORE;
			}
			break;
		}
	case PROPERTY_ID_SCORE_CLEAR:    //��������
		{
			//��������
			SCORE lCurrScore = pITargetUserItem->GetUserScore();
			if ( lCurrScore < 0)
			{
				//�û���Ϣ
				tagUserInfo * pUserInfo = pITargetUserItem->GetUserInfo();
				if(pUserInfo==NULL) return true;

				//�޸Ļ���
				pUserInfo->lScore=0;
			}
			break;
		}
	case PROPERTY_ID_ESCAPE_CLEAR:   //��������
		{
			//�û���Ϣ
			tagUserInfo * pUserInfo = pITargetUserItem->GetUserInfo();
			if(pUserInfo==NULL) return true;

			//�޸�������
			if(pUserInfo->dwFleeCount > 0)
			{
				pUserInfo->dwFleeCount=0;
			}

			break;
		}
	case PROPERTY_ID_GUARDKICK_CARD: //���߿�
		{
			//ʹ���ж�
			if((pUserProperty->wPropertyUseMark&PT_USE_MARK_GUARDKICK_CARD)!=0)
			{
				//��������
				DWORD  dwValidTime=pUserProperty->PropertyInfo[2].wPropertyCount*pUserProperty->PropertyInfo[2].dwValidNum;
				if(pUserProperty->PropertyInfo[2].dwEffectTime+dwValidTime<dwCurrentTime)
				{
					pUserProperty->PropertyInfo[2].dwEffectTime=dwCurrentTime;
					pUserProperty->PropertyInfo[2].wPropertyCount=pPropertySuccess->wItemCount;
					pUserProperty->PropertyInfo[2].dwValidNum=VALID_TIME_GUARDKICK_CARD;
				}
				else
				{
					//��Ŀ�ۼ�
					pUserProperty->PropertyInfo[2].wPropertyCount+=pPropertySuccess->wItemCount;
				}
			}
			else
			{
				//������Ϣ
				pUserProperty->PropertyInfo[2].dwEffectTime=dwCurrentTime;
				pUserProperty->PropertyInfo[2].wPropertyCount=pPropertySuccess->wItemCount;
				pUserProperty->PropertyInfo[2].dwValidNum=VALID_TIME_GUARDKICK_CARD;
				pUserProperty->wPropertyUseMark |= PT_USE_MARK_GUARDKICK_CARD;
			}

			break;
		}
	case PROPERTY_ID_POSSESS:        //�����
		{
			//ʹ���ж�
			if((pUserProperty->wPropertyUseMark&PT_USE_MARK_POSSESS)!=0)
			{
				//��������
				DWORD  dwValidTime=pUserProperty->PropertyInfo[3].wPropertyCount*pUserProperty->PropertyInfo[3].dwValidNum;
				if(pUserProperty->PropertyInfo[3].dwEffectTime+dwValidTime<dwCurrentTime)
				{
					pUserProperty->PropertyInfo[3].dwEffectTime=dwCurrentTime;
					pUserProperty->PropertyInfo[3].wPropertyCount=pPropertySuccess->wItemCount;
					pUserProperty->PropertyInfo[3].dwValidNum=VALID_TIME_POSSESS;
				}
				else
				{
					//��Ŀ�ۼ�
					pUserProperty->PropertyInfo[3].wPropertyCount+=pPropertySuccess->wItemCount;
				}
			}
			else
			{
				//������Ϣ
				pUserProperty->PropertyInfo[3].dwEffectTime=dwCurrentTime;
				pUserProperty->PropertyInfo[3].wPropertyCount=pPropertySuccess->wItemCount;
				pUserProperty->PropertyInfo[3].dwValidNum=VALID_TIME_POSSESS;
				pUserProperty->wPropertyUseMark |= PT_USE_MARK_POSSESS;
			}

			break;
		}
	case PROPERTY_ID_BLUERING_CARD:  //�����Ա
	case PROPERTY_ID_YELLOWRING_CARD://�����Ա
	case PROPERTY_ID_WHITERING_CARD: //�����Ա
	case PROPERTY_ID_REDRING_CARD:   //�����Ա
	case PROPERTY_ID_VIPROOM_CARD:   //VIP��Ա
		{
			//�û���Ϣ
			tagUserInfo * pUserInfo = pITargetUserItem->GetUserInfo();
			if(pUserInfo==NULL) return true;

			//���»�Ա
			pUserInfo->cbMemberOrder=pPropertySuccess->cbMemberOrder;
			
			//�޸�Ȩ��
			pITargetUserItem->ModifyUserRight(pPropertySuccess->dwUserRight,0);

			//������Ϣ
			SendPropertyEffect(pITargetUserItem);

			break;
		}
	default:  // ȫ������
		{			
			break;
		}
	}

	//���ѷ�ʽ
	if(pPropertySuccess->cbConsumeScore==FALSE)
	{
		pISourceUserItem->ModifyUserInsure(0,-pPropertySuccess->lConsumeGold,0);
	}

	//�����ж�
	if(pPropertySuccess->wPropertyIndex==PROPERTY_ID_TRUMPET|| pPropertySuccess->wPropertyIndex==PROPERTY_ID_TYPHON)
		return true;

	//��������
	CMD_GR_S_PropertySuccess PropertySuccess;
	ZeroMemory(&PropertySuccess,sizeof(PropertySuccess));

	//���ñ���
	PropertySuccess.cbRequestArea=pPropertySuccess->cbRequestArea;
	PropertySuccess.wItemCount=pPropertySuccess->wItemCount;
	PropertySuccess.wPropertyIndex=pPropertySuccess->wPropertyIndex;
	PropertySuccess.dwSourceUserID=pPropertySuccess->dwSourceUserID;
	PropertySuccess.dwTargetUserID=pPropertySuccess->dwTargetUserID;

	//������Ϣ
	if (pISourceUserItem!=NULL)
	{
		SendData(pISourceUserItem,MDM_GR_USER,SUB_GR_PROPERTY_SUCCESS,&PropertySuccess,sizeof(PropertySuccess));
	}

	//������Ϣ
	if (pITargetUserItem!=NULL && pITargetUserItem!=pISourceUserItem)
	{
		SendData(pITargetUserItem,MDM_GR_USER,SUB_GR_PROPERTY_SUCCESS,&PropertySuccess,sizeof(PropertySuccess));
	}

	//�㲥����
	if(GetPropertyType(pPropertySuccess->wPropertyIndex)==PT_TYPE_PRESENT)
		SendPropertyMessage(PropertySuccess.dwSourceUserID,PropertySuccess.dwTargetUserID,PropertySuccess.wPropertyIndex,
		PropertySuccess.wItemCount);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPropertyFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(DBO_GR_PropertyFailure));
	if(wDataSize!=sizeof(DBO_GR_PropertyFailure)) return false;

	//��ȡ����
	DBO_GR_PropertyFailure * pPropertyFailure = (DBO_GR_PropertyFailure *)pData;
	if(pPropertyFailure==NULL) return false;

	//��������
	WORD wBindIndex=LOWORD(dwContextID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	if(pBindParameter==NULL) return false;
	if(pBindParameter->pIServerUserItem==NULL) return false;

	//������Ϣ
	return SendPropertyFailure(pBindParameter->pIServerUserItem,pPropertyFailure->szDescribeString,0L,pPropertyFailure->cbRequestArea);
}

//ϵͳ��Ϣ
bool CAttemperEngineSink::OnDBSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(DBR_GR_SystemMessage));
	if(wDataSize!=sizeof(DBR_GR_SystemMessage)) return false;

	//��ȡ����
	DBR_GR_SystemMessage * pSystemMessage = (DBR_GR_SystemMessage *)pData;
	if(pSystemMessage==NULL) return false;

	//�ظ��ж�
	POSITION pos = m_SystemMessageList.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION tempPos = pos;
		tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);
		if(pTagSystemMessage->SystemMessage.cbMessageID == pSystemMessage->cbMessageID)
		{
			//��������
			pTagSystemMessage->dwLastTime=0;
			CopyMemory(&pTagSystemMessage->SystemMessage, pSystemMessage, sizeof(DBR_GR_SystemMessage));

			return true;
		}
	}

	//���ڱ���
	tagSystemMessage  *pSendMessage=new tagSystemMessage;
	ZeroMemory(pSendMessage, sizeof(tagSystemMessage));

	//���ñ���
	CopyMemory(&pSendMessage->SystemMessage,pSystemMessage, sizeof(DBR_GR_SystemMessage));

	//��¼��Ϣ
	m_SystemMessageList.AddTail(pSendMessage);

	return true;
}

//�������д�
bool CAttemperEngineSink::OnDBSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��ʼ����
	if(dwContextID==0xfffe)
	{
		m_WordsFilter.ResetSensitiveWordArray();
		m_pITimerEngine->KillTimer(IDI_LOAD_SENSITIVE_WORD);
		return true;			
	}

	//�������
	if(dwContextID==0xffff)
	{
		m_WordsFilter.FinishAdd();
		return true;
	}

	//�������д�
	const TCHAR *pWords=(const TCHAR*)pData;
	m_WordsFilter.AddSensitiveWords(pWords);
	return true;
}

//ע���¼�
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//ע��ʧ��
		{
			//��������
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//Ч�����
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//�رմ���
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//��ʾ��Ϣ
			if (lstrlen(pRegisterFailure->szDescribeString)>0)
			{
				CTraceService::TraceString(pRegisterFailure->szDescribeString,TraceLevel_Exception);
			}

			//�¼�֪ͨ
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�б��¼�
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//������Ϣ
		{
			//�����б�
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//��������
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);

			//��������
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//�������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//�����޸�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//��������
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//���÷���
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
	case SUB_CS_S_SERVER_REMOVE:	//����ɾ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//��������
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//��������
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//�������
		{
			//�����б�
			m_ServerListManager.CleanServerItem();

			//�¼�����
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnTCPSocketMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_COLLECT_REQUEST:	//�û�����
		{
			//��������
			CMD_CS_C_UserEnter UserEnter;
			ZeroMemory(&UserEnter,sizeof(UserEnter));

			//�����û�
			WORD wIndex=0;
			do
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
				if (pIServerUserItem==NULL) break;

				//���ñ���
				UserEnter.dwUserID=pIServerUserItem->GetUserID();
				UserEnter.dwGameID=pIServerUserItem->GetGameID();
				lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

				//������Ϣ
				UserEnter.cbGender=pIServerUserItem->GetGender();
				UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
				UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();
				UserEnter.cbAndroid=pIServerUserItem->IsAndroidUser();

				//��������
				ASSERT(m_pITCPSocketService!=NULL);
				m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));

			} while (true);

			//�㱨���
			m_bCollectUser=true;
			m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_FINISH);

			return true;
		}
	}

	return true;
}

//�������
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SYSTEM_MESSAGE:	//ϵͳ��Ϣ
		{
			//��Ϣ����
			SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);

			return true;
		}
	case SUB_CS_S_PROPERTY_TRUMPET:  //������Ϣ
		{
			//��������
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_PROPERTY_TRUMPET,pData,wDataSize,BG_COMPUTER);

			return true;
		}
	case SUB_CS_S_VIP_TRUMPET:
		{
			//��������
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

			//���»���
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

			//���»���
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

			// ���±���������
			printf("�������\n");
			if (pMatchSignUpRes->bIsSignUpSuc == true)
			{
				tagMatchUserInfoGameServer stMatchUserInfoGameServer = {0};
				stMatchUserInfoGameServer.dwUserID = pMatchSignUpRes->dwUserID;
				stMatchUserInfoGameServer.wGroupID = pMatchSignUpRes->wGroupID;
				stMatchUserInfoGameServer.lUserScore = pIServerUserItem->GetUserScore();
				
				printf("insert info userid = %ld, wgroupid = %d, userscore = %lld\n", pMatchSignUpRes->dwUserID, pMatchSignUpRes->wGroupID, stMatchUserInfoGameServer.lUserScore);

				m_vMatchUserInfo[pMatchSignUpRes->nMatchID - 1].push_back(stMatchUserInfoGameServer);
				printf("�û��������ݴ�С=%d,MatchID=%d,MatchIDsize=%d\n",m_vMatchUserInfo.size(),pMatchSignUpRes->nMatchID,m_vMatchUserInfo[pMatchSignUpRes->nMatchID - 1].size());

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

			// ����ҷ��ͱ����ɹ�����Ϣ
			SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_MATCH_SIGNUP_RES, &stUserMatchSignUPRes, sizeof(CMD_GRO_MatchSignupRes));

			return true;
		}
	case SUB_CS_S_MATCH_BEGIN:
		{
			printf("�յ�������ʼ��Ϣ\n");
			if (sizeof(CMD_CS_S_MatchBegin) != wDataSize)
			{
				return false;
			}

			// Begin Update UserScore Timer
			m_pITimerEngine->SetTimer(IDI_MATCH_GET_USERSCORE, TIME_MATCH_GET_USERSCORE_INTERVAL, (DWORD)(-1),NULL);

			CMD_CS_S_MatchBegin * pMatchBeginInfo = (CMD_CS_S_MatchBegin *)pData;
			m_cbMatchStatus = MATCH_STATUS_BEGIN;
			printf("������ʼ��Ϣ=%d\n",pMatchBeginInfo->nMatchID);
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
				printf("������������������=%d,��ҷ���=%d���Ƿ���Ҫ����=%d\n",stUserMatchEnd.nRanking,stUserMatchEnd.llScore,stUserMatchEnd.cbShare);
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

			printf("���ͷ����ı�����=%d\n",m_vMatchUserInfo[m_nCurrentMatchID -1].size());
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
						printf("����=%lld\n",stMatchScoreUpdate.lUserScore);
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
			//������ǰ������ң��в������ݵ�Ϊ�ѱ���
			tagBindParameter * pBindParameter=m_pNormalParameter;
			std::vector<IServerUserItem*> vNoSignUpUser;
			if (m_nCurrentMatchID==0)
			{
				m_nCurrentMatchID=pNotifyInfo->nMatchID;
			}

			printf("�յ�Э����ʾ��Ϣ\n");
			printf("��ǰ��Ϸ����=%d\n",m_nCurrentMatchID);
			printf("��ǰ��������=%d\n",m_vMatchUserInfo[m_nCurrentMatchID - 1].size());
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
					printf("������ҷ�����ʾ\n");
					pIServerUserItem = m_ServerUserManager.SearchUserItem(m_vMatchUserInfo[m_nCurrentMatchID-1][i].dwUserID);
					SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_MATCH_INFO_NOTIFY,&stMatchInfoNotify,sizeof(CMD_GRO_MatchInfoNotify));
				}
			}
			stMatchInfoNotify.bMatchStatus = false;
			for (int i=0;i<vNoSignUpUser.size();++i)
			{
				printf("û�б�����ҷ�����ʾ\n");
				SendData(vNoSignUpUser[i],MDM_GR_USER,SUB_GR_MATCH_INFO_NOTIFY,&stMatchInfoNotify,sizeof(CMD_GRO_MatchInfoNotify));
			}
			return true;
		}
	case SUB_CS_S_MATCH_NOTIFICATION_COUNTDOWN:
		{
			CMD_GPO_MatchTimeNotify * pNotifyInfo = (CMD_GPO_MatchTimeNotify *)pData;
			//������ǰ������ң��в������ݵ�Ϊ�ѱ���
			IServerUserItem * pIServerUserItem=NULL;
			printf("�յ�����5�뵹��ʱ��Ϣ�������������=%d, currentMatchID = %d\n",m_vMatchUserInfo[m_nCurrentMatchID-1].size(), m_nCurrentMatchID);
			for (int i=0;i<m_vMatchUserInfo[m_nCurrentMatchID-1].size();++i)
			{
				pIServerUserItem = m_ServerUserManager.SearchUserItem(m_vMatchUserInfo[m_nCurrentMatchID-1][i].dwUserID);
				printf("�յ�����5�뵹��ʱ��Ϣ���������id=%d\n",m_vMatchUserInfo[m_nCurrentMatchID-1][i].dwUserID);
				printf("����״̬=%d,ʣ������=%d������=%s\n",pNotifyInfo->cbMatchStatus,pNotifyInfo->nSecond,pNotifyInfo->szTitle);
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_MATCH_TIME_NOTIFY,pNotifyInfo,sizeof(CMD_GPO_MatchTimeNotify));
			}
			return true;
		}
	}

	return true;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_USERID:		//I D ��¼
		{
			return OnTCPNetworkSubLogonUserID(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_MOBILE:		//�ֻ���¼
		{
			return OnTCPNetworkSubLogonMobile(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubLogonAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_RULE:			//�û�����
		{
			return OnTCPNetworkSubUserRule(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_LOOKON:		//�û��Թ�
		{
			return OnTCPNetworkSubUserLookon(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_SITDOWN:		//�û�����
		{
			return OnTCPNetworkSubUserSitDown(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_STANDUP:		//�û�����
		{
			return OnTCPNetworkSubUserStandUp(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_CHAT:			//�û�����
		{
			return OnTCPNetworkSubUserChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_EXPRESSION:	//�û�����
		{
			return OnTCPNetworkSubUserExpression(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_WISPER_CHAT:		//�û�˽��
		{
			return OnTCPNetworkSubWisperChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_WISPER_EXPRESSION:	//˽�ı���
		{
			return OnTCPNetworkSubWisperExpression(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_PROPERTY_BUY:		//�������
		{
			return OnTCPNetworkSubPropertyBuy(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_PROPERTY_TRUMPET:   //���ȵ���
		{
			return OnTCPNetwordSubSendTrumpet(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_INVITE_REQ:    //�����û�
		{
			return OnTCPNetworkSubUserInviteReq(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_REPULSE_SIT:   //�ܾ�����
		{
			return OnTCPNetworkSubUserRepulseSit(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_KICK_USER:    //�߳��û�
		{
			return OnTCPNetworkSubMemberKickUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_INFO_REQ:     //�����û���Ϣ
		{
			return OnTCPNetworkSubUserInfoReq(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_CHAIR_REQ:    //�������λ��
		{
			return OnTCPNetworkSubUserChairReq(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_CHAIR_INFO_REQ: //���������û���Ϣ
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

//���д���
bool CAttemperEngineSink::OnTCPNetworkMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_INSURE_INFO:		//���в�ѯ
		{
			return OnTCPNetworkSubQueryInsureInfo(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SAVE_SCORE_REQUEST:		//�������
		{
			return OnTCPNetworkSubSaveScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TAKE_SCORE_REQUEST:		//ȡ������
		{
			return OnTCPNetworkSubTakeScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TRANSFER_SCORE_REQUEST:	//ת������
		{
			return OnTCPNetworkSubTransferScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_QUERY_USER_INFO_REQUEST:	//��ѯ�û�
		{
			return OnTCPNetworkSubQueryUserInfoRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GRR_USER_TRANSER_DRAGON:
		{
			if (wDataSize!=sizeof(CMD_GP_UserTransferDragon)) return false;

			//��ȡ�û�
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
			//Ч�����
			if (wDataSize!=sizeof(CMD_GR_UserUseDragon)) return false;

			//��ȡ�û�
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

			//Ͷ������
			m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_USE_DRAGON,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	}

	return false;
}

//������
bool CAttemperEngineSink::OnTCPNetworkMainManage(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_OPTION:		//��ѯ����
		{
			return OnTCPNetworkSubQueryOption(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_OPTION_SERVER:		//��������
		{
			return OnTCPNetworkSubOptionServer(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KILL_USER:          //�߳��û�
		{
			return OnTCPNetworkSubManagerKickUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LIMIT_USER_CHAT:	//��������
		{
			return OnTCPNetworkSubLimitUserChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KICK_ALL_USER:		//�߳��û�
		{
			return OnTCPNetworkSubKickAllUser(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SEND_MESSAGE:		//������Ϣ
		{
			return OnTCPNetworkSubSendMessage(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_DISMISSGAME:        //��ɢ��Ϸ
		{
			return OnTCPNetworkSubDismissGame(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//��������
bool CAttemperEngineSink::OnTCPNetworkMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ��Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//Ч��ӿ�
	ASSERT(m_pIGameMatchServiceManager!=NULL);
	if (m_pIGameMatchServiceManager==NULL) return false;

	//��Ϣ����
	return m_pIGameMatchServiceManager->OnEventSocketMatch(wSubCmdID,pData,wDataSize,pIServerUserItem,dwSocketID);
}

//��Ϸ����
bool CAttemperEngineSink::OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ��Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�������
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	return pTableFrame->OnEventSocketGame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//��ܴ���
bool CAttemperEngineSink::OnTCPNetworkMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ��Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�������
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	return pTableFrame->OnEventSocketFrame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubLogonUserID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonUserID));
	if (wDataSize<sizeof(CMD_GR_LogonUserID)) return false;

	//������Ϣ
	CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
	pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
	pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

	//����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//�ظ��ж�
	if ((pBindParameter==NULL)||(pIBindUserItem!=NULL))
	{ 
		ASSERT(FALSE);
		return false;
	}

	//�����ж�
	if(pLogonUserID->wKindID != m_pGameServiceOption->wKindID)
	{
		//����ʧ��
		SendLogonFailure(TEXT("�ܱ�Ǹ������Ϸ�����Ѿ��ر��ˣ�������������룡"),LOGON_FAIL_SERVER_INVALIDATION,dwSocketID);
		return true;
	}

	//�����˺����˲�����
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonUserID->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
			|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
		{
			SendRoomMessage(dwSocketID, TEXT("���˺����ڴ˷�����Ϸ���Ҳ������߳�������ѯ����Ա��"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
			return true;
		}
	}

	//�汾��Ϣ
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonUserID->dwPlazaVersion;
	pBindParameter->dwFrameVersion=pLogonUserID->dwFrameVersion;
	pBindParameter->dwProcessVersion=pLogonUserID->dwProcessVersion;

	//�����汾
	DWORD dwPlazaVersion=pLogonUserID->dwPlazaVersion;
	DWORD dwFrameVersion=pLogonUserID->dwFrameVersion;
	DWORD dwClientVersion=pLogonUserID->dwProcessVersion;
	if (PerformCheckVersion(dwPlazaVersion,dwFrameVersion,dwClientVersion,dwSocketID)==false) return true;

	//�л��ж�
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
	//��������
	DBR_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//��������
	LogonUserID.dwUserID=pLogonUserID->dwUserID;
	LogonUserID.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
	lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonUserID.dwUserID,DBR_GR_LOGON_USERID,dwSocketID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//�ֻ���¼
bool CAttemperEngineSink::OnTCPNetworkSubLogonMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonMobile));
	if (wDataSize<sizeof(CMD_GR_LogonMobile)) return false;

	//������Ϣ
	CMD_GR_LogonMobile * pLogonMobile=(CMD_GR_LogonMobile *)pData;
	pLogonMobile->szPassword[CountArray(pLogonMobile->szPassword)-1]=0;
	pLogonMobile->szMachineID[CountArray(pLogonMobile->szMachineID)-1]=0;

	//����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//�ظ��ж�
	if ((pBindParameter==NULL)||(pIBindUserItem!=NULL))
	{ 
		ASSERT(FALSE);
		return false;
	}

	//�����˺����˲�����
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonMobile->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
			|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
		{
			SendRoomMessage(dwSocketID, TEXT("���˺����ڴ˷�����Ϸ���Ҳ������߳�������ѯ����Ա��"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
			return false;
		}
	}

	//�汾��Ϣ
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->dwProcessVersion=pLogonMobile->dwProcessVersion;

	//�����汾
	DWORD dwClientVersion=pLogonMobile->dwProcessVersion;
	if (PerformCheckVersion(0L,0L,dwClientVersion,dwSocketID)==false) return true;

	//�л��ж�
	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonMobile->szPassword)==true))
	{
		SwitchUserItemConnect(pIServerUserItem,pLogonMobile->szMachineID,wBindIndex,pLogonMobile->cbDeviceType,pLogonMobile->wBehaviorFlags,pLogonMobile->wPageTableCount);
		return true;
	}

	//��������
	DBR_GR_LogonMobile LogonMobile;
	ZeroMemory(&LogonMobile,sizeof(LogonMobile));

	//��������
	LogonMobile.dwUserID=pLogonMobile->dwUserID;
	LogonMobile.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonMobile.szPassword,pLogonMobile->szPassword,CountArray(LogonMobile.szPassword));
	lstrcpyn(LogonMobile.szMachineID,pLogonMobile->szMachineID,CountArray(LogonMobile.szMachineID));
	LogonMobile.cbDeviceType=pLogonMobile->cbDeviceType;
	LogonMobile.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
	LogonMobile.wPageTableCount=pLogonMobile->wPageTableCount;

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(LogonMobile.dwUserID,DBR_GR_LOGON_MOBILE,dwSocketID,&LogonMobile,sizeof(LogonMobile));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonAccounts));
	if (wDataSize<=sizeof(CMD_GR_LogonAccounts)) return false;

	//������Ϣ
	CMD_GR_LogonAccounts * pLogonAccounts=(CMD_GR_LogonAccounts *)pData;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//�ظ��ж�
	if (pIBindUserItem!=NULL)
	{ 
		ASSERT(FALSE);
		return false;
	}

	//�����˺����˲�����
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonAccounts->szAccounts);
	if (pIServerUserItem!=NULL)
	{
		if((pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr!=0L))
			|| (!pIServerUserItem->IsAndroidUser() && (pBindParameter->dwClientAddr==0L)))
		{
			SendRoomMessage(dwSocketID, TEXT("���˺����ڴ˷�����Ϸ���Ҳ������߳�������ѯ����Ա��"), SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM,(pBindParameter->dwClientAddr==0L));
			return false;
		}
	}

	//�汾��Ϣ
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;
	pBindParameter->dwFrameVersion=pLogonAccounts->dwFrameVersion;
	pBindParameter->dwProcessVersion=pLogonAccounts->dwProcessVersion;

	//�����汾
	DWORD dwPlazaVersion=pLogonAccounts->dwPlazaVersion;
	DWORD dwFrameVersion=pLogonAccounts->dwFrameVersion;
	DWORD dwClientVersion=pLogonAccounts->dwProcessVersion;
	if (PerformCheckVersion(dwPlazaVersion,dwFrameVersion,dwClientVersion,dwSocketID)==false) return true;

	//�л��ж�
	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonAccounts->szPassword)==true))
	{
		SwitchUserItemConnect(pIServerUserItem,pLogonAccounts->szMachineID,wBindIndex);
		return true;
	}

	//��������
	DBR_GR_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//��������
	LogonAccounts.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserRule(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GR_UserRule));
	if (wDataSize<sizeof(CMD_GR_UserRule)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�����ж�
	//ASSERT(CServerRule::IsForfendGameRule(m_pGameServiceOption->dwServerRule)==false);
	if (CServerRule::IsForfendGameRule(m_pGameServiceOption->dwServerRule)==true) return true;

	//��Ϣ����
	tagUserRule * pUserRule=pIServerUserItem->GetUserRule();
	CMD_GR_UserRule * pCMDUserRule=(CMD_GR_UserRule *)pData;

	//�����־
	pUserRule->bLimitSameIP=((pCMDUserRule->cbRuleMask&UR_LIMIT_SAME_IP)>0);
	pUserRule->bLimitWinRate=((pCMDUserRule->cbRuleMask&UR_LIMIT_WIN_RATE)>0);
	pUserRule->bLimitFleeRate=((pCMDUserRule->cbRuleMask&UR_LIMIT_FLEE_RATE)>0);
	pUserRule->bLimitGameScore=((pCMDUserRule->cbRuleMask&UR_LIMIT_GAME_SCORE)>0);

	//��������
	pUserRule->szPassword[0]=0;
	pUserRule->wMinWinRate=pCMDUserRule->wMinWinRate;
	pUserRule->wMaxFleeRate=pCMDUserRule->wMaxFleeRate;
	pUserRule->lMaxGameScore=pCMDUserRule->lMaxGameScore;
	pUserRule->lMinGameScore=pCMDUserRule->lMinGameScore;

	//��������
	if (wDataSize>sizeof(CMD_GR_UserRule))
	{
		//��������
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pCMDUserRule+1,wDataSize-sizeof(CMD_GR_UserRule));

		//��ȡ����
		while (true)
		{
			//��ȡ����
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;

			//���ݷ���
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_TABLE_PASSWORD:		//��������
				{
					//Ч������
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(pUserRule->szPassword));

					//�����ж�
					ASSERT(CServerRule::IsForfendLockTable(m_pGameServiceOption->dwServerRule)==false);
					if (CServerRule::IsForfendLockTable(m_pGameServiceOption->dwServerRule)==true) break;

					//��������
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

//�û��Թ�
bool CAttemperEngineSink::OnTCPNetworkSubUserLookon(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserLookon));
	if (wDataSize!=sizeof(CMD_GR_UserLookon)) return false;

	//Ч������
	CMD_GR_UserLookon * pUserLookon=(CMD_GR_UserLookon *)pData;
	if (pUserLookon->wChairID>=m_pGameServiceAttrib->wChairCount) return false;
	if (pUserLookon->wTableID>=(WORD)m_TableFrameArray.GetCount()) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��Ϣ����
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	if ((wTableID==pUserLookon->wTableID)&&(wChairID==pUserLookon->wChairID)&&(cbUserStatus==US_LOOKON)) return true;

	//�û��ж�
	if (cbUserStatus==US_PLAYING)
	{
		SendRequestFailure(pIServerUserItem,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"),0);
		return true;
	}

	//�뿪����
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	//���´���
	CTableFrame * pTableFrame=m_TableFrameArray[pUserLookon->wTableID];
	pTableFrame->PerformLookonAction(pUserLookon->wChairID,pIServerUserItem);

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserSitDown(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserSitDown));
	if (wDataSize!=sizeof(CMD_GR_UserSitDown)) return false;

	//Ч������
	CMD_GR_UserSitDown * pUserSitDown=(CMD_GR_UserSitDown *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��Ϣ����
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//�ظ��ж�
	if ((pUserSitDown->wTableID<m_pGameServiceOption->wTableCount)&&(pUserSitDown->wChairID<m_pGameServiceAttrib->wChairCount))
	{
		CTableFrame * pTableFrame=m_TableFrameArray[pUserSitDown->wTableID];
		if (pTableFrame->GetTableUserItem(pUserSitDown->wChairID)==pIServerUserItem) return true;
	}

	//�û��ж�
	if (cbUserStatus==US_PLAYING)
	{
		SendRequestFailure(pIServerUserItem,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"),0);
		return true;
	}

	//�������
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
	{
		//����FALSEΪ ��ʱ����
		if(InsertDistribute(pIServerUserItem))
			return true;
	}

	//�뿪����
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	//������
	if(CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)&&(m_pGameServiceAttrib->wChairCount < MAX_CHAIR))
	{
		if(m_TableFrameArray[0]->EfficacyEnterTableScoreRule(0, pIServerUserItem))
			InsertWaitDistribute(pIServerUserItem);

		return true;
	}

	//�������
	WORD wRequestTableID=pUserSitDown->wTableID;
	WORD wRequestChairID=pUserSitDown->wChairID;

	//���ӵ���
	if (wRequestTableID>=m_TableFrameArray.GetCount())
	{
		//��ʼ����
		WORD wStartTableID=0;
		DWORD dwServerRule=m_pGameServiceOption->dwServerRule;
		if ((CServerRule::IsAllowAvertCheatMode(dwServerRule)==true)&&(m_pGameServiceAttrib->wChairCount<MAX_CHAIR)) wStartTableID=1;

		//��̬����
		bool bDynamicJoin=true;
		if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
		if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

		//Ѱ��λ��
		for (WORD i=wStartTableID;i<m_TableFrameArray.GetCount();i++)
		{
			//��Ϸ״̬
			if ((m_TableFrameArray[i]->IsGameStarted()==true)&&(bDynamicJoin==false))continue;

			//��ȡ��λ
			WORD wNullChairID=m_TableFrameArray[i]->GetNullChairID();

			//�������
			if (wNullChairID!=INVALID_CHAIR)
			{
				//���ñ���
				wRequestTableID=i;
				wRequestChairID=wNullChairID;

				break;
			}
		}

		//����ж�
		if ((wRequestTableID==INVALID_CHAIR)||(wRequestChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("��ǰ��Ϸ�����Ѿ�����Ϊ���ˣ���ʱû�п������������λ�ã����Ժ����ԣ�"),0);
			return true;
		}
	}

	//���ӵ���
	if (wRequestChairID>=m_pGameServiceAttrib->wChairCount)
	{
		//Ч�����
		ASSERT(wRequestTableID<m_TableFrameArray.GetCount());
		if (wRequestTableID>=m_TableFrameArray.GetCount()) return false;

		//���ҿ�λ
		wRequestChairID=m_TableFrameArray[wRequestTableID]->GetNullChairID();

		//����ж�
		if (wRequestChairID==INVALID_CHAIR)
		{
			SendRequestFailure(pIServerUserItem,TEXT("���ڴ���Ϸ����ʱû�п������������λ���ˣ���ѡ���������Ϸ����"),0);
			return true;
		}
	}

	//���´���
	CTableFrame * pTableFrame=m_TableFrameArray[wRequestTableID];
	pTableFrame->PerformSitDownAction(wRequestChairID,pIServerUserItem,pUserSitDown->szPassword);

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserStandUp(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserStandUp));
	if (wDataSize!=sizeof(CMD_GR_UserStandUp)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�������
	CMD_GR_UserStandUp * pUserStandUp=(CMD_GR_UserStandUp *)pData;

	//ȡ������
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

	//Ч������
	if (pUserStandUp->wChairID>=m_pGameServiceAttrib->wChairCount) return false;
	if (pUserStandUp->wTableID>=(WORD)m_TableFrameArray.GetCount()) return false;

	//��Ϣ����
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID!=pUserStandUp->wTableID)||(wChairID!=pUserStandUp->wChairID)) return true;

	//�û��ж�
	if ((pUserStandUp->cbForceLeave==FALSE)&&(pIServerUserItem->GetUserStatus()==US_PLAYING))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"),0);
		return true;
	}

	//�뿪����
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GR_USER, 500, NULL, 0);

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��������
	CMD_GR_C_UserChat * pUserChat=(CMD_GR_C_UserChat *)pData;

	//Ч�����
	ASSERT(wDataSize<=sizeof(CMD_GR_C_UserChat));
	ASSERT(wDataSize>=(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//Ч�����
	if (wDataSize>sizeof(CMD_GR_C_UserChat)) return false;
	if (wDataSize<(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//Ѱ���û�
	IServerUserItem * pIRecvUserItem=NULL;
	if (pUserChat->dwTargetUserID!=0)
	{
		pIRecvUserItem=m_ServerUserManager.SearchUserItem(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;
	}

	//״̬�ж�
	if ((CServerRule::IsForfendRoomChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("��Ǹ����ǰ����Ϸ�����ֹ�û��������죡"),SMT_CHAT);
		return true;
	}

	//Ȩ���ж�
	if (CUserRight::CanRoomChat(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("��Ǹ����û�д������Ե�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//������Ϣ
	CMD_GR_S_UserChat UserChat;
	ZeroMemory(&UserChat,sizeof(UserChat));

	//�ַ�����
	SensitiveWordFilter(pUserChat->szChatString,UserChat.szChatString,CountArray(UserChat.szChatString));
	
	//��������
	UserChat.dwChatColor=pUserChat->dwChatColor;
	UserChat.wChatLength=pUserChat->wChatLength;
	UserChat.dwSendUserID=pISendUserItem->GetUserID();
	UserChat.dwTargetUserID=pUserChat->dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//ת����Ϣ
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubUserExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_C_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_C_UserExpression)) return false;

	//��������
	CMD_GR_C_UserExpression * pUserExpression=(CMD_GR_C_UserExpression *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//Ѱ���û�
	IServerUserItem * pIRecvUserItem=NULL;
	if (pUserExpression->dwTargetUserID!=0)
	{
		pIRecvUserItem=m_ServerUserManager.SearchUserItem(pUserExpression->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;
	}

	//״̬�ж�
	if ((CServerRule::IsForfendRoomChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("��Ǹ����ǰ����Ϸ�����ֹ�û��������죡"),SMT_CHAT);
		return true;
	}

	//Ȩ���ж�
	if (CUserRight::CanRoomChat(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("��Ǹ����û�д������Ե�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//������Ϣ
	CMD_GR_S_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//��������
	UserExpression.wItemIndex=pUserExpression->wItemIndex;
	UserExpression.dwSendUserID=pISendUserItem->GetUserID();
	UserExpression.dwTargetUserID=pUserExpression->dwTargetUserID;

	//ת����Ϣ
	SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//�û�˽��
bool CAttemperEngineSink::OnTCPNetworkSubWisperChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��������
	CMD_GR_C_WisperChat * pWisperChat=(CMD_GR_C_WisperChat *)pData;

	//Ч�����
	ASSERT(wDataSize>=(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//Ч�����
	if (wDataSize<(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//Ѱ���û�
	IServerUserItem * pIRecvUserItem=m_ServerUserManager.SearchUserItem(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//״̬�ж�
	if ((CServerRule::IsForfendWisperChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("��Ǹ����ǰ����Ϸ�����ֹ�û�˽�ģ�"),SMT_CHAT);
		return true;
	}

	//ͬ���ж�
	if ((CServerRule::IsForfendWisperOnGame(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		//��������
		bool bForfend=true;
		WORD wTableIDSend=pISendUserItem->GetTableID();
		WORD wTableIDRecv=pIRecvUserItem->GetTableID();

		//�����ж�
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pISendUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&((wTableIDSend==INVALID_TABLE)||(wTableIDSend!=wTableIDRecv))) bForfend=false;

		//��ʾ��Ϣ
		if (bForfend==true)
		{
			SendRoomMessage(pISendUserItem,TEXT("��Ǹ������Ϸ���䲻��������Ϸ����ͬ�������˽�ģ�"),SMT_EJECT|SMT_CHAT);
			return true;
		}
	}

	//Ȩ���ж�
	if (CUserRight::CanWisper(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("��Ǹ����û�з���˽�ĵ�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//��������
	CMD_GR_S_WisperChat WisperChat;
	ZeroMemory(&WisperChat,sizeof(WisperChat));

	//�ַ�����
	SensitiveWordFilter(pWisperChat->szChatString,WisperChat.szChatString,CountArray(WisperChat.szChatString));

	//��������
	WisperChat.dwChatColor=pWisperChat->dwChatColor;
	WisperChat.wChatLength=pWisperChat->wChatLength;
	WisperChat.dwSendUserID=pISendUserItem->GetUserID();
	WisperChat.dwTargetUserID=pIRecvUserItem->GetUserID();
	WisperChat.wChatLength=CountStringBuffer(WisperChat.szChatString);

	//ת����Ϣ
	WORD wHeadSize=sizeof(WisperChat)-sizeof(WisperChat.szChatString);
	SendData(pISendUserItem,MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));
	SendData(pIRecvUserItem,MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));

	return true;
}

//�û�����
bool CAttemperEngineSink::OnTCPNetworkSubWisperExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_C_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_C_WisperExpression)) return false;

	//��������
	CMD_GR_C_WisperExpression * pWisperExpression=(CMD_GR_C_WisperExpression *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//Ѱ���û�
	IServerUserItem * pIRecvUserItem=m_ServerUserManager.SearchUserItem(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//״̬�ж�
	if ((CServerRule::IsForfendWisperChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("��Ǹ����ǰ����Ϸ�����ֹ�û�˽�ģ�"),SMT_CHAT);
		return true;
	}

	//ͬ���ж�
	if ((CServerRule::IsForfendWisperOnGame(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		//��������
		bool bForfend=true;
		WORD wTableIDSend=pISendUserItem->GetTableID();
		WORD wTableIDRecv=pIRecvUserItem->GetTableID();

		//�����ж�
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pISendUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&((wTableIDSend==INVALID_TABLE)||(wTableIDSend!=wTableIDRecv))) bForfend=false;

		//��ʾ��Ϣ
		if (bForfend==true)
		{
			SendRoomMessage(pISendUserItem,TEXT("��Ǹ������Ϸ���䲻��������Ϸ����ͬ�������˽�ģ�"),SMT_EJECT|SMT_CHAT);
			return true;
		}
	}

	//Ȩ���ж�
	if (CUserRight::CanWisper(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("��Ǹ����û�з���˽�ĵ�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//��������
	CMD_GR_S_WisperExpression WisperExpression;
	ZeroMemory(&WisperExpression,sizeof(WisperExpression));

	//��������
	WisperExpression.wItemIndex=pWisperExpression->wItemIndex;
	WisperExpression.dwSendUserID=pISendUserItem->GetUserID();
	WisperExpression.dwTargetUserID=pWisperExpression->dwTargetUserID;

	//ת����Ϣ
	SendData(pISendUserItem,MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));
	SendData(pIRecvUserItem,MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));

	return true;
}

//�������
bool CAttemperEngineSink::OnTCPNetworkSubPropertyBuy(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//�¼�����
	int cbResult = OnPropertyBuy(pData, wDataSize, dwSocketID);

	//����ж�
	if(cbResult == RESULT_ERROR) return false;
	if(cbResult == RESULT_FAIL) return true;
	if(cbResult == RESULT_SUCCESS) return true;

	return true;
}

//�������
int CAttemperEngineSink::OnPropertyBuy(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_C_PropertyBuy));
	if (wDataSize!=sizeof(CMD_GR_C_PropertyBuy)) return RESULT_ERROR;

	//��������
	CMD_GR_C_PropertyBuy * pPropertyBuy=(CMD_GR_C_PropertyBuy *)pData;

	//����Ч��
	ASSERT(pPropertyBuy->wItemCount>0);
	if (pPropertyBuy->wItemCount==0) return RESULT_ERROR;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return RESULT_ERROR;

	//Ŀ���û�
	IServerUserItem * pITargerUserItem=m_ServerUserManager.SearchUserItem(pPropertyBuy->dwTargetUserID);
	if (pITargerUserItem==NULL) return RESULT_ERROR;

	//��������
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		//������Ϣ
		SendPropertyFailure(pIServerUserItem,TEXT("�������䲻����ʹ�ô˹��ܣ�"),0L,pPropertyBuy->cbRequestArea);

		return RESULT_FAIL;
	}

	//��ϰ����
	if (m_pGameServiceOption->wServerType==GAME_GENRE_EDUCATE)
	{
		SendPropertyFailure(pIServerUserItem,TEXT("��ϰ���䲻����ʹ�ô˹��ܣ�"),0L,pPropertyBuy->cbRequestArea);
		return RESULT_FAIL;
	}

	//��ǰ�¼�
	if(OnEventPropertyBuyPrep(pPropertyBuy->cbRequestArea,pPropertyBuy->wPropertyIndex,pIServerUserItem,pITargerUserItem)==false)
		return RESULT_FAIL;

	//��������
	DBR_GR_PropertyRequest PropertyRequest;
	ZeroMemory(&PropertyRequest,sizeof(PropertyRequest));

	//���ҵ���
	tagPropertyInfo * pPropertyInfo=m_GamePropertyManager.SearchPropertyItem(pPropertyBuy->wPropertyIndex);
	if (pPropertyInfo==NULL)
	{
		SendPropertyFailure(pIServerUserItem,TEXT("������ĵ��߲����ڻ���ά���У��������Ա��ϵ��"),0L,pPropertyBuy->cbRequestArea);
		return RESULT_FAIL;
	}

	//���ѷ�ʽ
	if (pPropertyBuy->cbConsumeScore==TRUE)
	{
		//�����ж�
		ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
		if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return RESULT_FAIL;		

		//��������
		PropertyRequest.cbConsumeScore=TRUE;
		PropertyRequest.lFrozenedScore=pPropertyInfo->lPropertyGold*pPropertyBuy->wItemCount;

		//��Ա�ۿ�
		if (pIServerUserItem->GetMemberOrder()>0)
		{
			PropertyRequest.lFrozenedScore = PropertyRequest.lFrozenedScore*pPropertyInfo->wDiscount/100L;
		}

		//��������
		if (pIServerUserItem->FrozenedUserScore(PropertyRequest.lFrozenedScore)==false)
		{
			SendPropertyFailure(pIServerUserItem,TEXT("������Ϸ�����㣬���߹���ʧ�ܣ�"),0L,pPropertyBuy->cbRequestArea);
			return RESULT_FAIL;
		}
	}
	else
	{
		//���п۷�
		PropertyRequest.lFrozenedScore=0L;
		PropertyRequest.cbConsumeScore=FALSE;

		//��������
		SCORE lInsure = pIServerUserItem->GetUserInsure();
		SCORE lConsumeScore = pPropertyInfo->lPropertyGold*pPropertyBuy->wItemCount;

		//��Ա�ۿ�
		if (pIServerUserItem->GetMemberOrder()>0)
		{
			lConsumeScore = lConsumeScore*pPropertyInfo->wDiscount/100L;
		}

		//����У��
		if(lInsure < lConsumeScore)
		{
			SendPropertyFailure(pIServerUserItem,TEXT("���ı��չ����㣬������ٴι���"),0L,pPropertyBuy->cbRequestArea);
			return RESULT_FAIL;
		}		
	}

	//������Ϣ
	PropertyRequest.cbRequestArea=pPropertyBuy->cbRequestArea;
	PropertyRequest.wItemCount=pPropertyBuy->wItemCount;
	PropertyRequest.wPropertyIndex=pPropertyBuy->wPropertyIndex;
	PropertyRequest.dwSourceUserID=pIServerUserItem->GetUserID();
	PropertyRequest.dwTargetUserID=pITargerUserItem->GetUserID();

	//����Ȩ��
	if(PropertyRequest.wPropertyIndex==PROPERTY_ID_VIPROOM_CARD)
		PropertyRequest.dwUserRight |= UR_GAME_KICK_OUT_USER|UR_GAME_ENTER_VIP_ROOM;
	else if(PropertyRequest.wPropertyIndex>=PROPERTY_ID_BLUERING_CARD && PropertyRequest.wPropertyIndex <=PROPERTY_ID_REDRING_CARD)
		PropertyRequest.dwUserRight |= UR_GAME_KICK_OUT_USER;
	else
		PropertyRequest.dwUserRight=0;

	//ϵͳ��Ϣ
	PropertyRequest.wTableID=INVALID_TABLE;
	PropertyRequest.dwInoutIndex=pIServerUserItem->GetInoutIndex();
	PropertyRequest.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(PropertyRequest.szMachineID,pIServerUserItem->GetMachineID(),CountArray(PropertyRequest.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_PROPERTY_REQUEST,dwSocketID,&PropertyRequest,sizeof(PropertyRequest));

	return RESULT_SUCCESS;
}

//ʹ�õ���
bool CAttemperEngineSink::OnTCPNetwordSubSendTrumpet(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_C_SendTrumpet));
	if (wDataSize!=sizeof(CMD_GR_C_SendTrumpet)) return false;

	//��������
	CMD_GR_C_SendTrumpet * pSendTrumpet=(CMD_GR_C_SendTrumpet *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if(pIServerUserItem==NULL) return false;

	//����ṹ
	CMD_GR_C_PropertyBuy  PropertyBuy;
	PropertyBuy.cbRequestArea=pSendTrumpet->cbRequestArea;
	PropertyBuy.dwTargetUserID=pIServerUserItem->GetUserID();
	PropertyBuy.wPropertyIndex=pSendTrumpet->wPropertyIndex;
	PropertyBuy.cbConsumeScore=FALSE;
	PropertyBuy.wItemCount=1;
    
	//����
	int cbResult = OnPropertyBuy((void *)&PropertyBuy,sizeof(PropertyBuy),dwSocketID);

	//����ж�
	if(cbResult == RESULT_ERROR) return false;
	if(cbResult != RESULT_SUCCESS) return true;

	//��ȡ����
	tagUserProperty * pUserProperty = pIServerUserItem->GetUserProperty();

	//��������
	BYTE cbIndex=(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?2:3;

	//����ṹ
	CMD_GR_S_SendTrumpet  SendTrumpet;
	SendTrumpet.dwSendUserID=pIServerUserItem->GetUserID();
	SendTrumpet.wPropertyIndex=pSendTrumpet->wPropertyIndex;
	SendTrumpet.TrumpetColor=pSendTrumpet->TrumpetColor;
	ZeroMemory(SendTrumpet.szTrumpetContent,sizeof(SendTrumpet.szTrumpetContent));
	CopyMemory(SendTrumpet.szSendNickName,pIServerUserItem->GetNickName(),sizeof(SendTrumpet.szSendNickName));

	//�ַ�����
	SensitiveWordFilter(pSendTrumpet->szTrumpetContent,SendTrumpet.szTrumpetContent,CountArray(SendTrumpet.szTrumpetContent));

    //����ת��
	if(cbIndex==3)
	{
		//�㲥����
		WORD wUserIndex=0;
		CMD_CS_S_SendTrumpet * pSendTrumpet = (CMD_CS_S_SendTrumpet *)&SendTrumpet;
		if(m_pITCPSocketService)
		{
			m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_PROPERTY_TRUMPET,pSendTrumpet,sizeof(CMD_CS_S_SendTrumpet));
		}
	}

	//��Ϸת��
	if(cbIndex==2)
	{
		//��������
		m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_PROPERTY_TRUMPET,&SendTrumpet,sizeof(SendTrumpet),BG_COMPUTER);
	}

	return true;
}

//ʹ�õ���
bool CAttemperEngineSink::OnTCPNetworkSubVIPTrumpet(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
printf("%s %d\n", __FUNCTION__, __LINE__);
	//Ч�����
	if (wDataSize!=sizeof(CMD_GR_C_SendTrumpet))
	{
		printf("%s %d\n", __FUNCTION__, __LINE__);
		return false;
	}

	//��������
	CMD_GR_C_SendTrumpet * pSendTrumpet=(CMD_GR_C_SendTrumpet *)pData;

	//��ȡ�û�
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
	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_CHECK_TRUMPET,dwSocketID,&CheckTrumpet,sizeof(CheckTrumpet));

	return true;
}

//�����û�
bool CAttemperEngineSink::OnTCPNetworkSubUserInviteReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_GR_UserInviteReq));
	if (wDataSize!=sizeof(CMD_GR_UserInviteReq)) return false;

	//��Ϣ����
	CMD_GR_UserInviteReq * pUserInviteReq=(CMD_GR_UserInviteReq *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ч��״̬
	if (pUserInviteReq->wTableID==INVALID_TABLE) return true;
	if (pIServerUserItem->GetTableID()!=pUserInviteReq->wTableID) return true;
	if (pIServerUserItem->GetUserStatus()==US_PLAYING) return true;
	if (pIServerUserItem->GetUserStatus()==US_OFFLINE) return true;

	//Ŀ���û�
	IServerUserItem * pITargetUserItem=m_ServerUserManager.SearchUserItem(pUserInviteReq->dwUserID);
	if (pITargetUserItem==NULL) return true;
	if (pITargetUserItem->GetUserStatus()==US_PLAYING) return true;

	//������Ϣ
	CMD_GR_UserInvite UserInvite;
	memset(&UserInvite,0,sizeof(UserInvite));
	UserInvite.wTableID=pUserInviteReq->wTableID;
	UserInvite.dwUserID=pIServerUserItem->GetUserID();
	SendData(pITargetUserItem,MDM_GR_USER,SUB_GR_USER_INVITE,&UserInvite,sizeof(UserInvite));

	return true;
}

//�ܾ�����
bool CAttemperEngineSink::OnTCPNetworkSubUserRepulseSit(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_GR_UserRepulseSit));
	if (wDataSize!=sizeof(CMD_GR_UserRepulseSit)) return false;

	//��Ϣ����
	CMD_GR_UserRepulseSit * pUserRepulseSit=(CMD_GR_UserRepulseSit *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//��ȡ����
	CTableFrame * pTableFrame=m_TableFrameArray[pUserRepulseSit->wTableID];
	if (pTableFrame->IsGameStarted()==true) return true;

	//��ȡ�û�
	IServerUserItem * pRepulseIServerUserItem = pTableFrame->GetTableUserItem(pUserRepulseSit->wChairID);
	if (pRepulseIServerUserItem==NULL) return true;
	if(pRepulseIServerUserItem->GetUserID() != pUserRepulseSit->dwRepulseUserID)return true;

	//������Ϣ
	TCHAR szDescribe[256]=TEXT("");
	lstrcpyn(szDescribe,TEXT("��������������˲�����ͬ����Ϸ��"),CountArray(szDescribe));
	SendRoomMessage(pRepulseIServerUserItem,szDescribe,SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME);

	//�������
	pTableFrame->PerformStandUpAction(pRepulseIServerUserItem);

	return true;
}

//�߳�����
bool CAttemperEngineSink::OnTCPNetworkSubMemberKickUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_KickUser));
	if (wDataSize!=sizeof(CMD_GR_KickUser)) return false;

	//��������
	CMD_GR_KickUser * pKickUser=(CMD_GR_KickUser *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//Ŀ���û�
	IServerUserItem * pITargetUserItem = m_ServerUserManager.SearchUserItem(pKickUser->dwTargetUserID);
	if(pITargetUserItem==NULL) return true;

	//�û�Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMemberOrder()>pITargetUserItem->GetMemberOrder()));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMemberOrder()<=pITargetUserItem->GetMemberOrder())) return false;

	//Ȩ���ж�
	ASSERT(CUserRight::CanKillOutUser(pIServerUserItem->GetUserRight())==true);
	if (CUserRight::CanKillOutUser(pIServerUserItem->GetUserRight())==false) return false;

	//������Ϸ
	if(m_pGameServiceAttrib->wChairCount >= MAX_CHAIR)
	{
		//������Ϣ
		SendRoomMessage(pIServerUserItem,TEXT("�ܱ�Ǹ��������Ϸ�������ˣ�"),SMT_EJECT);
		return true;
	}

	//�û�״̬
	if(pITargetUserItem->GetUserStatus()==US_PLAYING)
	{
		//��������
		TCHAR szMessage[256]=TEXT("");
		_sntprintf(szMessage,CountArray(szMessage),TEXT("������� [ %s ] ������Ϸ��,�����ܽ����߳���Ϸ��"),pITargetUserItem->GetNickName());

		//������Ϣ
		SendRoomMessage(pIServerUserItem,szMessage,SMT_EJECT);
		return true;
	}

	//�����ж�
	if((pITargetUserItem->GetUserProperty()->wPropertyUseMark&PT_USE_MARK_GUARDKICK_CARD)!=0)
	{
		//��������
		DWORD dwCurrentTime=(DWORD)time(NULL);
		tagUserProperty * pUserProperty = pITargetUserItem->GetUserProperty();

		//ʱЧ�ж�
		DWORD dwValidTime=pUserProperty->PropertyInfo[2].wPropertyCount*pUserProperty->PropertyInfo[2].dwValidNum;
		if(pUserProperty->PropertyInfo[2].dwEffectTime+dwValidTime>dwCurrentTime)
		{
			//��������
			TCHAR szMessage[256]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("������� [ %s ] ����ʹ�÷��߿�,���޷������߳���Ϸ��"),pITargetUserItem->GetNickName());

			//������Ϣ
			SendRoomMessage(pIServerUserItem,szMessage,SMT_EJECT);

			return true; 
		}
		else
			pUserProperty->wPropertyUseMark &= ~PT_USE_MARK_GUARDKICK_CARD;
	}

	//��������
	WORD wTargerTableID = pITargetUserItem->GetTableID();
	if(wTargerTableID != INVALID_TABLE)
	{
		//�������
		TCHAR szMessage[64]=TEXT("");
		_sntprintf(szMessage,CountArray(szMessage),TEXT("���ѱ�%s�������ӣ�"),pIServerUserItem->GetNickName());

		//������Ϣ
		SendGameMessage(pITargetUserItem,szMessage,SMT_CHAT|SMT_CLOSE_GAME);

		CTableFrame * pTableFrame=m_TableFrameArray[wTargerTableID];
		if (pTableFrame->PerformStandUpAction(pITargetUserItem)==false) return true;
	}

	return true;
}

//�����û���Ϣ
bool CAttemperEngineSink::OnTCPNetworkSubUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserInfoReq));
	if (wDataSize!=sizeof(CMD_GR_UserInfoReq)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_UserInfoReq * pUserInfoReq = (CMD_GR_UserInfoReq *)pData;
	WORD wCurDeskPos = pIServerUserItem->GetMobileUserDeskPos();
	WORD wMaxDeskPos = m_pGameServiceOption->wTableCount-pIServerUserItem->GetMobileUserDeskCount();

	//����Ч��
	if(pUserInfoReq->wTablePos > wMaxDeskPos) pUserInfoReq->wTablePos = wMaxDeskPos;

	//������Ϣ
	pIServerUserItem->SetMobileUserDeskPos(pUserInfoReq->wTablePos);
	
	//������Ϣ
	SendViewTableUserInfoPacketToMobileUser(pIServerUserItem,pUserInfoReq->dwUserIDReq);

	return true;
}

//�������λ��
bool CAttemperEngineSink::OnTCPNetworkSubUserChairReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û�״̬
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		//ʧ��
		m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"),REQUEST_FAILURE_NORMAL);
		return true;
	}

	//��������
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//����ͬ��
		if(i == pIServerUserItem->GetTableID())continue;

		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//��Ч����
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;

		//�뿪����
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
		}

		//�û�����
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//����ͬ��
	if(pIServerUserItem->GetTableID() != INVALID_TABLE)
	{
		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
		if (pTableFrame->IsGameStarted()==false && pTableFrame->IsTableLocked()==false)
		{
			//��Ч����
			WORD wChairID=pTableFrame->GetRandNullChairID();
			if (wChairID!=INVALID_CHAIR)
			{
				//�뿪����
				if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
				{
					CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
					if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
				}

				//�û�����
				pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
				return true;
			}
		}
	}

	//ʧ��
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("û�ҵ��ɽ������Ϸ����"),REQUEST_FAILURE_NORMAL);
	return true;
}

//���������û���Ϣ
bool CAttemperEngineSink::OnTCPNetworkSubChairUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_ChairUserInfoReq));
	if (wDataSize!=sizeof(CMD_GR_ChairUserInfoReq)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_ChairUserInfoReq * pUserInfoReq = (CMD_GR_ChairUserInfoReq *)pData;
	if(pUserInfoReq->wTableID == INVALID_TABLE) return true;
	if(pUserInfoReq->wTableID >= m_pGameServiceOption->wTableCount)return true;

	//������Ϣ
	WORD wChairCout = m_TableFrameArray[pUserInfoReq->wTableID]->GetChairCount();
	for(WORD wIndex = 0; wIndex < wChairCout; wIndex++)
	{
		//��ȡ�û�
		if(pUserInfoReq->wChairID != INVALID_CHAIR && wIndex != pUserInfoReq->wChairID)continue;
		IServerUserItem * pTagerIServerUserItem=m_TableFrameArray[pUserInfoReq->wTableID]->GetTableUserItem(wIndex);
		if(pTagerIServerUserItem==NULL)continue;

		//��������
		BYTE cbBuffer[SOCKET_TCP_PACKET]={0};
		tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));
		tagUserInfo *pUserInfo = pTagerIServerUserItem->GetUserInfo();

		//�û�����
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//�û�����
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//�û�״̬
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//�û�����
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//�û��ɼ�
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

		//������Ϣ
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//������Ϣ
		WORD wHeadSize=sizeof(tagMobileUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//��ѯ����
bool CAttemperEngineSink::OnTCPNetworkSubQueryInsureInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_C_QueryInsureInfoRequest));
	if (wDataSize!=sizeof(CMD_GR_C_QueryInsureInfoRequest)) return false;

	//�����ж�
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)||(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH));
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0&&(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_C_QueryInsureInfoRequest * pQueryInsureInfoRequest = (CMD_GR_C_QueryInsureInfoRequest *)pData;

	//��������
	DBR_GR_QueryInsureInfo QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//��������
	QueryInsureInfo.cbActivityGame=pQueryInsureInfoRequest->cbActivityGame;
	QueryInsureInfo.dwUserID=pIServerUserItem->GetUserID();
	QueryInsureInfo.dwClientAddr=pIServerUserItem->GetClientAddr();

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(QueryInsureInfo.dwUserID,DBR_GR_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return true;
}

//�������
bool CAttemperEngineSink::OnTCPNetworkSubSaveScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_C_SaveScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_SaveScoreRequest)) return false;

	//�����ж�
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)||(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH));
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0&&(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//��������
	CMD_GR_C_SaveScoreRequest * pSaveScoreRequest=(CMD_GR_C_SaveScoreRequest *)pData;

	//Ч�����
	ASSERT(pSaveScoreRequest->lSaveScore>0L);
	if (pSaveScoreRequest->lSaveScore<=0L) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�����ж�
	if(pSaveScoreRequest->cbActivityGame == FALSE && CServerRule::IsForfendSaveInRoom(m_pGameServiceOption->dwServerRule))
	{
		//��������
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ������������ʧ�ܣ�"),0L,pSaveScoreRequest->cbActivityGame);
		return true;
	}

	//�����ж�
	if(pSaveScoreRequest->cbActivityGame == TRUE && CServerRule::IsForfendSaveInGame(m_pGameServiceOption->dwServerRule))
	{
		//��������
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ��Ϸ���������ʧ�ܣ�"),0L,pSaveScoreRequest->cbActivityGame);
		return true;
	}

	//��������
	SCORE lConsumeQuota=0L;
	SCORE lUserWholeScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();

	//��ȡ�޶�
	if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		lConsumeQuota=m_TableFrameArray[wTableID]->QueryConsumeQuota(pIServerUserItem);
	}
	else
	{
		lConsumeQuota=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	}

	//�޶��ж�
	if (pSaveScoreRequest->lSaveScore>lConsumeQuota)
	{
		if (lConsumeQuota<lUserWholeScore)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������������Ϸ�У���Ϸ�ҿɴ�����Ϊ %I64d���������ʧ�ܣ�"),lConsumeQuota);

			//��������
			SendInsureFailure(pIServerUserItem,szDescribe,0L,pSaveScoreRequest->cbActivityGame);
		}
		else
		{
			//��������
			SendInsureFailure(pIServerUserItem,TEXT("������Ϸ�����㣬�������ʧ�ܣ�"),0L,pSaveScoreRequest->cbActivityGame);
		}

		return true;
	}

	//��������
	if (pIServerUserItem->FrozenedUserScore(pSaveScoreRequest->lSaveScore)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	DBR_GR_UserSaveScore UserSaveScore;
	ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

	//��������
	UserSaveScore.cbActivityGame=pSaveScoreRequest->cbActivityGame;
	UserSaveScore.dwUserID=pIServerUserItem->GetUserID();
	UserSaveScore.lSaveScore=pSaveScoreRequest->lSaveScore;
	UserSaveScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserSaveScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserSaveScore.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

	return true;
}

//ȡ������
bool CAttemperEngineSink::OnTCPNetworkSubTakeScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_C_TakeScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_TakeScoreRequest)) return false;

	//�����ж�
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)||(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH));
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0&&(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//��������
	CMD_GR_C_TakeScoreRequest * pTakeScoreRequest=(CMD_GR_C_TakeScoreRequest *)pData;
	pTakeScoreRequest->szInsurePass[CountArray(pTakeScoreRequest->szInsurePass)-1]=0;

	//Ч�����
	ASSERT(pTakeScoreRequest->lTakeScore>0L);
	if (pTakeScoreRequest->lTakeScore<=0L) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�����ж�
	if(pTakeScoreRequest->cbActivityGame == FALSE && CServerRule::IsForfendTakeInRoom(m_pGameServiceOption->dwServerRule))
	{
		//��������
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ����ȡ�ȡ������ʧ�ܣ�"),0L,pTakeScoreRequest->cbActivityGame);
		return true;
	}

	//�����ж�
	if(pTakeScoreRequest->cbActivityGame == TRUE && CServerRule::IsForfendTakeInGame(m_pGameServiceOption->dwServerRule))
	{
		//��������
		SendInsureFailure(pIServerUserItem,TEXT("�˷����ֹ��Ϸȡ�ȡ������ʧ�ܣ�"),0L,pTakeScoreRequest->cbActivityGame);
		return true;
	}

	//��������
	DBR_GR_UserTakeScore UserTakeScore;
	ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

	//��������
	UserTakeScore.cbActivityGame=pTakeScoreRequest->cbActivityGame;
	UserTakeScore.dwUserID=pIServerUserItem->GetUserID();
	UserTakeScore.lTakeScore=pTakeScoreRequest->lTakeScore;
	UserTakeScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserTakeScore.szPassword,pTakeScoreRequest->szInsurePass,CountArray(UserTakeScore.szPassword));
	lstrcpyn(UserTakeScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTakeScore.szMachineID));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

	return true;
}

//ת������
bool CAttemperEngineSink::OnTCPNetworkSubTransferScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GP_C_TransferScoreRequest));
	if (wDataSize!=sizeof(CMD_GP_C_TransferScoreRequest)) return false;

	//�����ж�
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)||(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH));
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0&&(m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0) return false;

	//��������
	CMD_GP_C_TransferScoreRequest * pTransferScoreRequest=(CMD_GP_C_TransferScoreRequest *)pData;
	pTransferScoreRequest->szNickName[CountArray(pTransferScoreRequest->szNickName)-1]=0;
	pTransferScoreRequest->szInsurePass[CountArray(pTransferScoreRequest->szInsurePass)-1]=0;

	//Ч�����
	ASSERT(pTransferScoreRequest->lTransferScore>0L);
	if (pTransferScoreRequest->lTransferScore<=0L) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	DBR_GR_UserTransferScore UserTransferScore;
	ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

	//��������
	UserTransferScore.cbActivityGame=pTransferScoreRequest->cbActivityGame;
	UserTransferScore.dwUserID=pIServerUserItem->GetUserID();
	UserTransferScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	UserTransferScore.cbByNickName=pTransferScoreRequest->cbByNickName;
	UserTransferScore.lTransferScore=pTransferScoreRequest->lTransferScore;
	lstrcpyn(UserTransferScore.szNickName,pTransferScoreRequest->szNickName,CountArray(UserTransferScore.szNickName));
	lstrcpyn(UserTransferScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTransferScore.szMachineID));
	lstrcpyn(UserTransferScore.szPassword,pTransferScoreRequest->szInsurePass,CountArray(UserTransferScore.szPassword));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

	return true;
}

//��ѯ�û�����
bool CAttemperEngineSink::OnTCPNetworkSubQueryUserInfoRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_C_QueryUserInfoRequest));
	if (wDataSize!=sizeof(CMD_GR_C_QueryUserInfoRequest)) return false;

	//�����ж�
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//��������
	CMD_GR_C_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GR_C_QueryUserInfoRequest *)pData;
	pQueryUserInfoRequest->szNickName[CountArray(pQueryUserInfoRequest->szNickName)-1]=0;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	DBR_GR_QueryTransferUserInfo QueryTransferUserInfo;
	ZeroMemory(&QueryTransferUserInfo,sizeof(QueryTransferUserInfo));

	//��������
	QueryTransferUserInfo.cbActivityGame=pQueryUserInfoRequest->cbActivityGame;
	QueryTransferUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
	lstrcpyn(QueryTransferUserInfo.szNickName,pQueryUserInfoRequest->szNickName,CountArray(QueryTransferUserInfo.szNickName));
	QueryTransferUserInfo.dwUserID=pIServerUserItem->GetUserID();

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_QUERY_TRANSFER_USER_INFO,dwSocketID,&QueryTransferUserInfo,sizeof(QueryTransferUserInfo));

	return true;
}

//��ѯ����
bool CAttemperEngineSink::OnTCPNetworkSubQueryOption(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//��������
	CMD_GR_OptionCurrent OptionCurrent;
	ZeroMemory(&OptionCurrent,sizeof(OptionCurrent));

	//�ҽ�����
	OptionCurrent.ServerOptionInfo.wKindID=m_pGameServiceOption->wKindID;
	OptionCurrent.ServerOptionInfo.wNodeID=m_pGameServiceOption->wNodeID;
	OptionCurrent.ServerOptionInfo.wSortID=m_pGameServiceOption->wSortID;

	//��������
	OptionCurrent.ServerOptionInfo.wRevenueRatio=m_pGameServiceOption->wRevenueRatio;
	OptionCurrent.ServerOptionInfo.lServiceScore=m_pGameServiceOption->lServiceScore;
	OptionCurrent.ServerOptionInfo.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	OptionCurrent.ServerOptionInfo.lMinTableScore=m_pGameServiceOption->lMinTableScore;
	OptionCurrent.ServerOptionInfo.lMinEnterScore=m_pGameServiceOption->lMinEnterScore;
	OptionCurrent.ServerOptionInfo.lMaxEnterScore=m_pGameServiceOption->lMaxEnterScore;

	//��Ա����
	OptionCurrent.ServerOptionInfo.cbMinEnterMember=m_pGameServiceOption->cbMinEnterMember;
	OptionCurrent.ServerOptionInfo.cbMaxEnterMember=m_pGameServiceOption->cbMaxEnterMember;

	//��������
	OptionCurrent.ServerOptionInfo.dwServerRule=m_pGameServiceOption->dwServerRule;
	lstrcpyn(OptionCurrent.ServerOptionInfo.szServerName,m_pGameServiceOption->szServerName,CountArray(OptionCurrent.ServerOptionInfo.szServerName));

	//�������
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_ON_GAME;

	//�������
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_LOOKON;

	//���й���
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_GAME;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_GAME;

	//��������
	//OptionCurrent.dwRuleMask|=SR_RECORD_GAME_TRACK;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_RULE;
	OptionCurrent.dwRuleMask|=SR_FORFEND_LOCK_TABLE;
	OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_SIMULATE;

	//�������
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_DYNAMIC_JOIN;
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_ATTEND;
	//if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_OFFLINE_TRUSTEE;

	//ģʽ����
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0) OptionCurrent.dwRuleMask|=SR_RECORD_GAME_SCORE;
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0) OptionCurrent.dwRuleMask|=SR_IMMEDIATE_WRITE_SCORE;

	//��������
	SendData(pIServerUserItem,MDM_GR_MANAGE,SUB_GR_OPTION_CURRENT,&OptionCurrent,sizeof(OptionCurrent));

	return true;
}

//��������
bool CAttemperEngineSink::OnTCPNetworkSubOptionServer(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_ServerOption));
	if (wDataSize!=sizeof(CMD_GR_ServerOption)) return false;

	//��������
	CMD_GR_ServerOption * pServerOption=(CMD_GR_ServerOption *)pData;
	tagServerOptionInfo * pServerOptionInfo=&pServerOption->ServerOptionInfo;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//��������
	bool bModifyServer=false;

	//�ҽӽڵ�
	if (m_pGameServiceOption->wNodeID!=pServerOptionInfo->wNodeID)
	{
		bModifyServer=true;
		m_pGameServiceOption->wNodeID=pServerOptionInfo->wNodeID;
	}

	//�ҽ�����
	if ((pServerOptionInfo->wKindID!=0)&&(m_pGameServiceOption->wKindID!=pServerOptionInfo->wKindID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wKindID=pServerOptionInfo->wKindID;
	}
	
	//�ҽ�����
	if ((pServerOptionInfo->wSortID!=0)&&(m_pGameServiceOption->wSortID!=pServerOptionInfo->wSortID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wSortID=pServerOptionInfo->wSortID;
	}

	//��������
	if ((pServerOptionInfo->szServerName[0]!=0)&&(lstrcmp(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName)!=0))
	{
		bModifyServer=true;
		lstrcpyn(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName,CountArray(m_pGameServiceOption->szServerName));
	}

	//˰������
	m_pGameServiceOption->wRevenueRatio=pServerOptionInfo->wRevenueRatio;
	m_pGameServiceOption->lServiceScore=pServerOptionInfo->lServiceScore;

	//��������
	m_pGameServiceOption->lRestrictScore=pServerOptionInfo->lRestrictScore;
	m_pGameServiceOption->lMinTableScore=pServerOptionInfo->lMinTableScore;
	m_pGameServiceOption->lMinEnterScore=pServerOptionInfo->lMinEnterScore;
	m_pGameServiceOption->lMaxEnterScore=pServerOptionInfo->lMaxEnterScore;

	//��Ա����
	m_pGameServiceOption->cbMinEnterMember=pServerOptionInfo->cbMinEnterMember;
	m_pGameServiceOption->cbMaxEnterMember=pServerOptionInfo->cbMaxEnterMember;

	//�������
	CServerRule::SetForfendGameChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendRoomChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperOnGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperOnGame(pServerOptionInfo->dwServerRule));

	//�������
	CServerRule::SetForfendRoomEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameLookon(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameLookon(pServerOptionInfo->dwServerRule));

	//���й���
	CServerRule::SetForfendTakeInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendTakeInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInGame(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInGame(pServerOptionInfo->dwServerRule));

	//��������
	CServerRule::SetRecordGameTrack(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameTrack(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameRule(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameRule(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendLockTable(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendLockTable(pServerOptionInfo->dwServerRule));

	//��̬����
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE)
	{
		CServerRule::SetAllowDynamicJoin(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowDynamicJoin(pServerOptionInfo->dwServerRule));
	}

	//��������
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE)
	{
		CServerRule::SetAllowAndroidAttend(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowAndroidAttend(pServerOptionInfo->dwServerRule));
	}

	//�����й�
	if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE)
	{
		CServerRule::SetAllowOffLineTrustee(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowOffLineTrustee(pServerOptionInfo->dwServerRule));
	}

	//��¼�ɼ�
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0)
	{
		CServerRule::SetRecordGameScore(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameScore(pServerOptionInfo->dwServerRule));
	}

	//����д��
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0)
	{
		CServerRule::SetImmediateWriteScore(m_pGameServiceOption->dwServerRule,CServerRule::IsImmediateWriteScore(pServerOptionInfo->dwServerRule));
	}

	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->RectifyServiceParameter();

	//�����޸�
	if (bModifyServer==true)
	{
		//��������
		CMD_CS_C_ServerModify ServerModify;
		ZeroMemory(&ServerModify,sizeof(ServerModify));

		//����˿�
		ServerModify.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();

		//������Ϣ
		ServerModify.wKindID=m_pGameServiceOption->wKindID;
		ServerModify.wNodeID=m_pGameServiceOption->wNodeID;
		ServerModify.wSortID=m_pGameServiceOption->wSortID;
		ServerModify.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		ServerModify.dwFullCount=m_pGameServiceOption->wMaxPlayer-RESERVE_USER_COUNT;
		lstrcpyn(ServerModify.szServerName,m_pGameServiceOption->szServerName,CountArray(ServerModify.szServerName));
		lstrcpyn(ServerModify.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(ServerModify.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_MODIFY,&ServerModify,sizeof(ServerModify));
	}

	//������Ϣ
	SendRoomMessage(pIServerUserItem,TEXT("��ǰ��Ϸ����������ġ�����ֵ��״̬���������޸ĳɹ�"),SMT_CHAT|SMT_EJECT);

	//�����Ϣ
	TCHAR szBuffer[128]=TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("Զ���޸ķ�������֪ͨ ����Ա %s [ %ld ]"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserID());

	//�����Ϣ
	CTraceService::TraceString(szBuffer,TraceLevel_Info);

	return true;
}

//�߳��û�
bool CAttemperEngineSink::OnTCPNetworkSubManagerKickUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_KickUser));
	if (wDataSize!=sizeof(CMD_GR_KickUser)) return false;

	//��������
	CMD_GR_KickUser * pKickUser=(CMD_GR_KickUser *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//�û�Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//Ȩ���ж�
	ASSERT(CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==false) return false;

	//Ŀ���û�
	IServerUserItem * pITargetUserItem = m_ServerUserManager.SearchUserItem(pKickUser->dwTargetUserID);
	if(pITargetUserItem==NULL) return true;

	////�û�״̬
	//if(pITargetUserItem->GetUserStatus()==US_PLAYING) return true;

	//�߳���¼
	m_KickUserItemMap[pITargetUserItem->GetUserID()]=(DWORD)time(NULL);

	//��������
	WORD wTargerTableID = pITargetUserItem->GetTableID();
	if(wTargerTableID != INVALID_TABLE)
	{
		//������Ϣ
		SendGameMessage(pITargetUserItem,TEXT("���ѱ�����Ա�������ӣ�"),SMT_CHAT|SMT_CLOSE_GAME);

		CTableFrame * pTableFrame=m_TableFrameArray[wTargerTableID];
		if (pTableFrame->PerformStandUpAction(pITargetUserItem)==false) return true;
	}

	//����֪ͨ
	LPCTSTR pszMessage=TEXT("���ѱ�����Ա�������Ϸ���䣡");
	SendRoomMessage(pITargetUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

	pITargetUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);

	return true;
}

//��������
bool CAttemperEngineSink::OnTCPNetworkSubLimitUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_GR_LimitUserChat));
	if (wDataSize!=sizeof(CMD_GR_LimitUserChat)) return false;

	//��Ϣ����
	CMD_GR_LimitUserChat * pLimitUserChat=(CMD_GR_LimitUserChat *)pData;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ȩ���ж�
	//ASSERT(CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==true);
	//if (CMasterRight::CanLimitUserChat(pIServerUserItem->GetMasterRight())==false) return false;

	//Ŀ���û�
	IServerUserItem * pITargerUserItem=m_ServerUserManager.SearchUserItem(pLimitUserChat->dwTargetUserID);
	if (pITargerUserItem==NULL) return true;

	//��������
	DWORD dwAddRight = 0, dwRemoveRight = 0;

	//��������
	if (pLimitUserChat->cbLimitFlags==OSF_ROOM_CHAT)
	{
		if (CMasterRight::CanLimitRoomChat(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_ROOM_CHAT;
		else
			dwRemoveRight |= UR_CANNOT_ROOM_CHAT;
	}

	//��Ϸ����
	if (pLimitUserChat->cbLimitFlags==OSF_GAME_CHAT)
	{
		if (CMasterRight::CanLimitGameChat(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_GAME_CHAT;
		else
			dwRemoveRight |= UR_CANNOT_GAME_CHAT;
	}

	//����˽��
	if (pLimitUserChat->cbLimitFlags==OSF_ROOM_WISPER)
	{
		if (CMasterRight::CanLimitWisper(pIServerUserItem->GetMasterRight())==false) return false;

		if( pLimitUserChat->cbLimitValue == TRUE )
			dwAddRight |= UR_CANNOT_WISPER;
		else
			dwRemoveRight |= UR_CANNOT_WISPER;
	}

	//��������
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

		//����֪ͨ
		CMD_GR_ConfigUserRight cur = {0};
		cur.dwUserRight = pITargerUserItem->GetUserRight();

		SendData( pITargerUserItem,MDM_GR_CONFIG,SUB_GR_CONFIG_USER_RIGHT,&cur,sizeof(cur) );

		//������Ϣ
		SendRoomMessage(pIServerUserItem,TEXT("�û�����Ȩ�����óɹ���"),SMT_CHAT);
	}
	else return false;

	return true;
}

//�߳������û�
bool CAttemperEngineSink::OnTCPNetworkSubKickAllUser(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��Ϣ����
	CMD_GR_KickAllUser * pKillAllUser=(CMD_GR_KickAllUser *)pData;

	//Ч������
	ASSERT(wDataSize<=sizeof(CMD_GR_KickAllUser));
	if( wDataSize > sizeof(CMD_GR_KickAllUser) ) return false;
	ASSERT(wDataSize==CountStringBuffer(pKillAllUser->szKickMessage));
	if (wDataSize!=CountStringBuffer(pKillAllUser->szKickMessage)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ȩ���ж�
	ASSERT(CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanKillUser(pIServerUserItem->GetMasterRight())==false) return false;

	//��ɢ������Ϸ
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if ( !pTableFrame->IsGameStarted() ) continue;

		pTableFrame->DismissGame();
	}

	tagBindParameter *pBindParameter = m_pNormalParameter;
	for( INT i = 0; i < m_pGameServiceOption->wMaxPlayer; i++ )
	{
		//Ŀ¼�û�
		IServerUserItem * pITargerUserItem= pBindParameter->pIServerUserItem;
		if (pITargerUserItem==NULL || pITargerUserItem==pIServerUserItem ) 
		{
			pBindParameter++;
			continue;
		}

		//������Ϣ
		SendRoomMessage(pITargerUserItem,pKillAllUser->szKickMessage,SMT_CHAT|SMT_EJECT|SMT_CLOSE_LINK|SMT_CLOSE_ROOM);

		pBindParameter++;
	} 

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnTCPNetworkSubSendMessage(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ȩ���ж�
	ASSERT(CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==false) return false;

	//��Ϣ����
	return SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);
}

//��ɢ��Ϸ
bool CAttemperEngineSink::OnTCPNetworkSubDismissGame(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_GR_DismissGame));
	if (wDataSize!=sizeof(CMD_GR_DismissGame)) return false;

	//��ȡ�û�
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
	if (pIServerUserItem==NULL) return false;

	//Ȩ���ж�
	ASSERT(CMasterRight::CanDismissGame(pIServerUserItem->GetMasterRight())==true);
	if (CMasterRight::CanDismissGame(pIServerUserItem->GetMasterRight())==false) return false;

	//��Ϣ����
	CMD_GR_DismissGame * pDismissGame=(CMD_GR_DismissGame *)pData;

	//Ч������
	if(pDismissGame->wDismissTableNum >= m_TableFrameArray.GetCount()) return true;

	//��ɢ��Ϸ
	CTableFrame *pTableFrame=m_TableFrameArray[pDismissGame->wDismissTableNum];
	if(pTableFrame)
	{
		if(pTableFrame->IsGameStarted()) pTableFrame->DismissGame();
	}

	return true;
}

//�û���¼
VOID CAttemperEngineSink::OnEventUserLogon(IServerUserItem * pIServerUserItem, bool bAlreadyOnLine)
{
	//��ȡ����
	WORD wBindIndex=pIServerUserItem->GetBindIndex();
	bool bAndroidUser=pIServerUserItem->IsAndroidUser();
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//��¼����
	if (pIServerUserItem->IsMobileUser()==false)
	{
		//��������
		CMD_GR_LogonSuccess LogonSuccess;
		//CMD_GR_ConfigServer ConfigServer;
		ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));
		//ZeroMemory(&ConfigServer,sizeof(ConfigServer));

		//��¼�ɹ�
		LogonSuccess.dwUserRight=pIServerUserItem->GetUserRight();
		LogonSuccess.dwMasterRight=pIServerUserItem->GetMasterRight();
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

		//��������
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

		//��������Ϣ
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

		//�б�����
		WORD wConfigColumnHead=sizeof(m_DataConfigColumn)-sizeof(m_DataConfigColumn.ColumnItem);
		WORD wConfigColumnInfo=m_DataConfigColumn.cbColumnCount*sizeof(m_DataConfigColumn.ColumnItem[0]);
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_COLUMN,&m_DataConfigColumn,wConfigColumnHead+wConfigColumnInfo);

		//��������
		WORD wConfigPropertyHead=sizeof(m_DataConfigProperty)-sizeof(m_DataConfigProperty.PropertyInfo);
		WORD wConfigPropertyInfo=m_DataConfigProperty.cbPropertyCount*sizeof(m_DataConfigProperty.PropertyInfo[0]);
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_PROPERTY,&m_DataConfigProperty,wConfigPropertyHead+wConfigPropertyInfo);

		//�������
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_FINISH,NULL,0);

		//�������
		SendUserInfoPacket(pIServerUserItem,pBindParameter->dwSocketID);

		//�����û�
		WORD wUserIndex=0;
		IServerUserItem * pIServerUserItemSend=NULL;
		while (true)
		{
			pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
			if (pIServerUserItemSend==NULL) break;
			if (pIServerUserItemSend==pIServerUserItem) continue;
			SendUserInfoPacket(pIServerUserItemSend,pBindParameter->dwSocketID);
		}

		//����״̬
		CMD_GR_TableInfo TableInfo;
		TableInfo.wTableCount=(WORD)m_TableFrameArray.GetCount();
		ASSERT(TableInfo.wTableCount<CountArray(TableInfo.TableStatusArray));
		for (WORD i=0;i<TableInfo.wTableCount;i++)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[i];
			TableInfo.TableStatusArray[i].cbTableLock=pTableFrame->IsTableLocked()?TRUE:FALSE;
			TableInfo.TableStatusArray[i].cbPlayStatus=pTableFrame->IsTableStarted()?TRUE:FALSE;
		}

		//����״̬
		WORD wHeadSize=sizeof(TableInfo)-sizeof(TableInfo.TableStatusArray);
		WORD wSendSize=wHeadSize+TableInfo.wTableCount*sizeof(TableInfo.TableStatusArray[0]);
		SendData(pBindParameter->dwSocketID,MDM_GR_STATUS,SUB_GR_TABLE_INFO,&TableInfo,wSendSize);

		//����֪ͨ
		if (bAlreadyOnLine==false)
		{
			SendUserInfoPacket(pIServerUserItem,INVALID_DWORD);
		}

		//��¼���
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);

		//��ӭ��Ϣ
		if (bAndroidUser==false)
		{
			////������ʾ
			//TCHAR szMessage[128]=TEXT("");
			//_sntprintf(szMessage,CountArray(szMessage),TEXT("��ӭ�����롰%s����Ϸ���䣬ף����Ϸ��죡"),m_pGameServiceAttrib->szGameName);

			////������Ϣ
			//SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);

			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�װ����û�����ӭ������669������Ϸƽ̨��\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("\n���Ʋ�����Ϸ���ܾ�������Ϸ�� \nע�����ұ�����������ƭ�ϵ��� \n�ʶ���Ϸ���ԣ�������Ϸ���� \n������ʱ�䣬���ܽ������\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("��ǿ��������ֹ���ţ��Ͻ�������Ϸƽ̨���жĲ���Ϊ��������г���绷����\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�ֻ��沶�����ȵ�½��׿ϵͳ�ֻ���ƽ�壬ƻ��ϵͳ�ֻ���ƽ�塣"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("֧�ֵ����ֻ�ͬ����Ϸ����ʱ��س���669���㣬��������ɣ�\n�ֻ������ص�ַ��m.45669.com\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("1.2ϲ���콵��Ʒ��ʢ���������ѣ�ƽ�壬�ֻ��������С�"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���������ֻ�����һ��ǵ��Զ���ң���Ϸ�л����κ��㶼�л������ɱ�ƽ̨���͵Ļ��ѣ�ƽ�壬�ֻ���"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
			_sntprintf(szMessage,CountArray(szMessage),TEXT("������Ϸ���䱶��Խ�ߣ���ƷԽ���Ŷ��\n"));
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
		}
	}
	else
	{
		//��������
		CMD_GR_ConfigServer ConfigServer;
		ZeroMemory(&ConfigServer,sizeof(ConfigServer));

		//��������
		ConfigServer.wTableCount=m_pGameServiceOption->wTableCount;
		ConfigServer.wChairCount=m_pGameServiceAttrib->wChairCount;
		ConfigServer.wServerType=m_pGameServiceOption->wServerType;
		ConfigServer.dwServerRule=m_pGameServiceOption->dwServerRule;
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_SERVER,&ConfigServer,sizeof(ConfigServer));

		//�������
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_FINISH,NULL,0);

		//�������
		SendViewTableUserInfoPacketToMobileUser(pIServerUserItem,pIServerUserItem->GetUserID());

		//Ⱥ���û�
		SendUserInfoPacket(pIServerUserItem,INVALID_DWORD);

		//��¼���
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);

		//������¼
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

	//��������
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

//�û��뿪
VOID CAttemperEngineSink::OnEventUserLogout(IServerUserItem * pIServerUserItem, DWORD dwLeaveReason)
{
	//��������
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//��ȡ�ɼ�
	pIServerUserItem->QueryRecordInfo(LeaveGameServer.RecordInfo);
	pIServerUserItem->DistillVariation(LeaveGameServer.VariationInfo);

	//�û���Ϣ
	LeaveGameServer.dwLeaveReason=dwLeaveReason;
	LeaveGameServer.dwUserID=pIServerUserItem->GetUserID();
	LeaveGameServer.dwInoutIndex=pIServerUserItem->GetInoutIndex();
	LeaveGameServer.dwOnLineTimeCount=(DWORD)(time(NULL))-pIServerUserItem->GetLogonTime();

	//������Ϣ
	LeaveGameServer.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(LeaveGameServer.szMachineID,pIServerUserItem->GetMachineID(),CountArray(LeaveGameServer.szMachineID));
	lstrcpyn(LeaveGameServer.szNickName,pIServerUserItem->GetNickName(),CountArray(LeaveGameServer.szNickName));
	lstrcpyn(LeaveGameServer.szServerName,m_pGameServiceOption->szServerName,CountArray(LeaveGameServer.szServerName));

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer), TRUE);
	
	//�����û�
	if (m_bCollectUser==true)
	{
		//��������
		CMD_CS_C_UserLeave UserLeave;
		ZeroMemory(&UserLeave,sizeof(UserLeave));

		//���ñ���
		UserLeave.dwUserID=pIServerUserItem->GetUserID();

		//������Ϣ
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_LEAVE,&UserLeave,sizeof(UserLeave));
	}

	//֪�����������˳���Ϸ
	if(m_pIGameMatchServiceManager!=NULL)m_pIGameMatchServiceManager->OnUserQuitGame(pIServerUserItem, 0);

	//����ɾ��
	DeleteWaitDistribute(pIServerUserItem);

	//ɾ���û�
	m_ServerUserManager.DeleteUserItem(pIServerUserItem);

	return;
}

//������Ϸ��
bool CAttemperEngineSink::PerformUnlockScore(DWORD dwUserID, DWORD dwInoutIndex, DWORD dwLeaveReason)
{
	//��������
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//���ñ���
	LeaveGameServer.dwUserID=dwUserID;
	LeaveGameServer.dwInoutIndex=dwInoutIndex;
	LeaveGameServer.dwLeaveReason=dwLeaveReason;

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(dwUserID,DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer));

	return true;
}

//�汾���
bool CAttemperEngineSink::PerformCheckVersion(DWORD dwPlazaVersion, DWORD dwFrameVersion, DWORD dwClientVersion, DWORD dwSocketID)
{
	//��������
	bool bMustUpdateClient=false;
	bool bAdviceUpdateClient=false;

	//��Ϸ�汾
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

	//����֪ͨ
	if ((bMustUpdateClient==true)||(bAdviceUpdateClient==true))
	{
		//��������
		CMD_GR_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//��������
		UpdateNotify.cbMustUpdatePlaza=false;
		UpdateNotify.cbMustUpdateClient=bMustUpdateClient;
		UpdateNotify.cbAdviceUpdateClient=bAdviceUpdateClient;

		//��ǰ�汾
		UpdateNotify.dwCurrentPlazaVersion=VERSION_PLAZA;
		UpdateNotify.dwCurrentFrameVersion=VERSION_FRAME;
		UpdateNotify.dwCurrentClientVersion=m_pGameServiceAttrib->dwClientVersion;

		//������Ϣ
		SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//��ֹ�ж�
		if (bMustUpdateClient==true)
		{
			printf("ShutDownSocket %d\n", __LINE__);
			//m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return true;
		}
	}

	return true;
}

//�л�����
bool CAttemperEngineSink::SwitchUserItemConnect(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	//Ч�����
	ASSERT((pIServerUserItem!=NULL)&&(wTargetIndex!=INVALID_WORD));
	if ((pIServerUserItem==NULL)||(wTargetIndex==INVALID_WORD)) return false;

	//�Ͽ��û�
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//����֪ͨ
		LPCTSTR pszMessage=TEXT("��ע�⣬�����ʺ�����һ�ط������˴���Ϸ���䣬�������뿪��");
		SendRoomMessage(pIServerUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

		//�󶨲���
		WORD wSourceIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pSourceParameter=GetBindParameter(wSourceIndex);

		if (pSourceParameter == NULL) return false;

		//�����
		ASSERT((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem));
		if ((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem)) pSourceParameter->pIServerUserItem=NULL;

		//�Ͽ��û�
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

	//״̬�л�
	bool bIsOffLine=false;
	if (pIServerUserItem->GetUserStatus()==US_OFFLINE)
	{
		//��������
		WORD wTableID=pIServerUserItem->GetTableID();
		WORD wChairID=pIServerUserItem->GetChairID();

		//����״̬
		bIsOffLine=true;
		pIServerUserItem->SetUserStatus(US_PLAYING,wTableID,wChairID);
	}

	//�����ж�
	LPCTSTR pszMachineID=pIServerUserItem->GetMachineID();
	bool bSameMachineID=(lstrcmp(pszMachineID,szMachineID)==0);

	//��������
	bool bAndroidUser=(wTargetIndex>=INDEX_ANDROID);
	tagBindParameter * pTargetParameter=GetBindParameter(wTargetIndex);

	//�����û�
	pTargetParameter->pIServerUserItem=pIServerUserItem;
	pIServerUserItem->SetUserParameter(pTargetParameter->dwClientAddr,wTargetIndex,szMachineID,bAndroidUser,false);

	//�ֻ���ʶ
	if(pTargetParameter->cbClientKind==CLIENT_KIND_MOBILE)
	{
		pIServerUserItem->SetMobileUser(true);
		SetMobileUserParameter(pIServerUserItem,cbDeviceType,wBehaviorFlags,wPageTableCount);
	}

	//��¼�¼�
	OnEventUserLogon(pIServerUserItem,true);

	//��ȫ��ʾ
	if ((bAndroidUser==false)&&(bIsOffLine==false)&&(bSameMachineID==false))
	{
		SendRoomMessage(pIServerUserItem,TEXT("��ע�⣬�����ʺ�����һ�ط������˴���Ϸ���䣬�Է������뿪��"),SMT_EJECT|SMT_CHAT|SMT_GLOBAL);
	}

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::SendLogonFailure(LPCTSTR pszString, LONG lErrorCode, DWORD dwSocketID)
{
	//��������
	CMD_GR_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));

	//��������
	LogonFailure.lErrorCode=lErrorCode;
	lstrcpyn(LogonFailure.szDescribeString,pszString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);

	//��������
	SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FAILURE,&LogonFailure,wHeadSize+wDataSize);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::SendInsureFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszString, LONG lErrorCode,BYTE cbActivityGame)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_S_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	UserInsureFailure.cbActivityGame=cbActivityGame;
	UserInsureFailure.lErrorCode=lErrorCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pszString,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);

	//��������
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDataSize);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode)
{
	//��������
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//���ñ���
	RequestFailure.lErrorCode=lErrorCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::SendPropertyFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode,WORD wRequestArea)
{
	//��������
	CMD_GR_PropertyFailure PropertyFailure;
	ZeroMemory(&PropertyFailure,sizeof(PropertyFailure));

	//���ñ���
	PropertyFailure.lErrorCode=lErrorCode;
	PropertyFailure.wRequestArea=wRequestArea;
	lstrcpyn(PropertyFailure.szDescribeString,pszDescribe,CountArray(PropertyFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(PropertyFailure.szDescribeString);
	WORD wHeadSize=sizeof(PropertyFailure)-sizeof(PropertyFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_PROPERTY_FAILURE,&PropertyFailure,wHeadSize+wDataSize);

	return true;
}

//�����û�
bool CAttemperEngineSink::SendUserInfoPacket(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));

	//�û�����
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwGroupID=pUserInfo->dwGroupID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//�û�����
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserInfo->cbMasterOrder;

	//�û�״̬
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//�û�����
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwUserMedal=pUserInfo->dwUserMedal;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;
	pUserInfoHead->lLoveLiness=pUserInfo->lLoveLiness;

	//�û�����
	pUserInfoHead->lGrade=pUserInfo->lGrade;
	pUserInfoHead->lInsure=pUserInfo->lInsure;

	//�û��ɼ�
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
	pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

	//������Ϣ
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);
	SendPacket.AddPacket(pUserInfo->szGroupName,DTP_GR_GROUP_NAME);
	SendPacket.AddPacket(pUserInfo->szUnderWrite,DTP_GR_UNDER_WRITE);

	//��������
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

//�㲥����
bool CAttemperEngineSink::SendPropertyMessage(DWORD dwSourceID,DWORD dwTargerID,WORD wPropertyIndex,WORD wPropertyCount)
{
	//����ṹ
	CMD_GR_S_PropertyMessage  PropertyMessage;
	PropertyMessage.wPropertyIndex=wPropertyIndex;
	PropertyMessage.dwSourceUserID=dwSourceID;
	PropertyMessage.dwTargerUserID=dwTargerID;
	PropertyMessage.wPropertyCount=wPropertyCount;

	//�����û�
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

//����ЧӦ
bool CAttemperEngineSink::SendPropertyEffect(IServerUserItem * pIServerUserItem)
{
	//����У��
	if(pIServerUserItem==NULL) return false;

	//����ṹ
	CMD_GR_S_PropertyEffect  PropertyEffect;
	PropertyEffect.wUserID =pIServerUserItem->GetUserID();
	PropertyEffect.cbMemberOrder=pIServerUserItem->GetMemberOrder();

	//�����û�
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

//�����¼�
bool CAttemperEngineSink::OnEventPropertyBuyPrep(WORD cbRequestArea,WORD wPropertyIndex,IServerUserItem *pISourceUserItem,IServerUserItem *pTargetUserItem)
{
	//Ŀ�����
	if ( pTargetUserItem == NULL )
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("����ʧ�ܣ���Ҫ���͵�����Ѿ��뿪��"), 0L,cbRequestArea);

		return false;
	}

	//�����ж�
	if ( (m_pGameServiceOption->wServerType == GAME_GENRE_GOLD || m_pGameServiceOption->wServerType == GAME_GENRE_MATCH ) && 
		(wPropertyIndex== PROPERTY_ID_SCORE_CLEAR||wPropertyIndex==PROPERTY_ID_TWO_CARD||wPropertyIndex == PROPERTY_ID_FOUR_CARD||wPropertyIndex == PROPERTY_ID_POSSESS) )
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("�˷��䲻����ʹ�ô˵���,����ʧ��"), 0L,cbRequestArea);

		return false;
	}

	//���ҵ���
	tagPropertyInfo * pPropertyInfo=m_GamePropertyManager.SearchPropertyItem(wPropertyIndex);

	//��ЧЧ��
	if(pPropertyInfo==NULL)
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("�˵��߻�δ����,����ʧ�ܣ�"), 0L,cbRequestArea);

		return false;
	}

	//�Լ�ʹ��
    if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_MESELF)==0 && pISourceUserItem==pTargetUserItem) 
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("�˵��߲����Լ�ʹ��,����ʧ�ܣ�"), 0L,cbRequestArea);

		return false;
	}

	//���ʹ��
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_PLAYER)==0 && pISourceUserItem!=pTargetUserItem) 
	{
		//������Ϣ
		SendPropertyFailure(pISourceUserItem,TEXT("�˵��߲������͸����,ֻ���Լ�ʹ��,����ʧ�ܣ�"), 0L,cbRequestArea);

		return false;
	}

	//�Թ۷�Χ
	if((pPropertyInfo->wIssueArea&PT_SERVICE_AREA_LOOKON)==0)  
	{
		//��������
		WORD wTableID = pTargetUserItem->GetTableID();
		if(wTableID!=INVALID_TABLE)
		{
			//��������
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//��ȡ����
			CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	
			//ö���û�
			do
			{
				//��ȡ�û�
				pIServerUserItem=pTableFrame->EnumLookonUserItem(wEnumIndex++);
				if( pIServerUserItem==NULL) break;
				if( pIServerUserItem==pTargetUserItem )
				{
					//������Ϣ
					SendPropertyFailure(pISourceUserItem,TEXT("�˵��߲������͸��Թ��û�,����ʧ�ܣ�"), 0L,cbRequestArea);

					return false;
				}
			} while (true);
		}
	}

	//�����ж�
	switch(wPropertyIndex)
	{
	case PROPERTY_ID_SCORE_CLEAR :			//��������
		{
			//��������
			SCORE lCurrScore = pTargetUserItem->GetUserScore();
			if( lCurrScore >= 0)
			{
				//��������
				TCHAR szMessage[128]=TEXT("");
				if ( pISourceUserItem==pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("�����ڵĻ����Ѿ��ǷǸ���������Ҫʹ�ø���������ߣ�"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]���ڵĻ����Ѿ��ǷǸ���������Ҫʹ�ø���������ߣ�"), pTargetUserItem->GetNickName());

				//������Ϣ
				SendPropertyFailure(pISourceUserItem,szMessage, 0L,cbRequestArea);

				return false;
			}
			break;
		}
	case PROPERTY_ID_ESCAPE_CLEAR :			 //��������
		{
			//��������
			DWORD dwCurrFleeCount = pTargetUserItem->GetUserInfo()->dwFleeCount;
			if ( dwCurrFleeCount==0 )
			{
				//��������
				TCHAR szMessage[128]=TEXT("");		
				if ( pISourceUserItem == pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("�����ڵ��������Ѿ�Ϊ0������Ҫʹ������������ߣ�"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]���ڵ��������Ѿ�Ϊ0������Ҫʹ������������ߣ�"), pTargetUserItem->GetNickName());

				//������Ϣ
				SendPropertyFailure(pISourceUserItem,szMessage,0L,cbRequestArea);

				return false;
			}
			break;
		}
	}

	return true;
}

//���û�
IServerUserItem * CAttemperEngineSink::GetBindUserItem(WORD wBindIndex)
{
	//��ȡ����
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//��ȡ�û�
	if (pBindParameter!=NULL)
	{
		return pBindParameter->pIServerUserItem;
	}

	//�������
	ASSERT(FALSE);

	return NULL;
}

//�󶨲���
tagBindParameter * CAttemperEngineSink::GetBindParameter(WORD wBindIndex)
{
	//��Ч����
	if (wBindIndex==INVALID_WORD) return NULL;

	//��������
	if (wBindIndex<m_pGameServiceOption->wMaxPlayer)
	{
		return m_pNormalParameter+wBindIndex;
	}

	//��������
	if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		return m_pAndroidParameter+(wBindIndex-INDEX_ANDROID);
	}

	//�������
	ASSERT(FALSE);

	return NULL;
}

//��������
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

//���û���
bool CAttemperEngineSink::InitAndroidUser()
{
	//��������
	tagAndroidUserParameter AndroidUserParameter;
	ZeroMemory(&AndroidUserParameter,sizeof(AndroidUserParameter));

	//���ò���
	AndroidUserParameter.pGameParameter=m_pGameParameter;
	AndroidUserParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	AndroidUserParameter.pGameServiceOption=m_pGameServiceOption;

	//�������
	AndroidUserParameter.pITimerEngine=m_pITimerEngine;
	AndroidUserParameter.pIServerUserManager=&m_ServerUserManager;
	AndroidUserParameter.pIGameServiceManager=m_pIGameServiceManager;
	AndroidUserParameter.pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent);

	//���ö���
	if (m_AndroidUserManager.InitAndroidUser(AndroidUserParameter)==false)
	{
		return false;
	}

	return true;
}

//��������
bool CAttemperEngineSink::InitTableFrameArray()
{
	//���Ӳ���
	tagTableFrameParameter TableFrameParameter;
	ZeroMemory(&TableFrameParameter,sizeof(TableFrameParameter));

	//�ں����
	TableFrameParameter.pITimerEngine=m_pITimerEngine;
	TableFrameParameter.pIKernelDataBaseEngine=m_pIKernelDataBaseEngine;
	TableFrameParameter.pIRecordDataBaseEngine=m_pIRecordDataBaseEngine;

	//�������
	TableFrameParameter.pIMainServiceFrame=this;
	TableFrameParameter.pIAndroidUserManager=&m_AndroidUserManager;
	TableFrameParameter.pIGameServiceManager=m_pIGameServiceManager;

	//���ò���
	TableFrameParameter.pGameParameter=m_pGameParameter;
	TableFrameParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	TableFrameParameter.pGameServiceOption=m_pGameServiceOption;

	if(m_pIGameMatchServiceManager!=NULL)
		TableFrameParameter.pIGameMatchServiceManager=QUERY_OBJECT_PTR_INTERFACE(m_pIGameMatchServiceManager,IUnknownEx);

	//��������
	m_TableFrameArray.SetSize(m_pGameServiceOption->wTableCount);
	ZeroMemory(m_TableFrameArray.GetData(),m_pGameServiceOption->wTableCount*sizeof(CTableFrame *));

	//��������
	for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		//��������
		m_TableFrameArray[i]=new CTableFrame;

		//��������
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

//��������
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//�������
bool CAttemperEngineSink::InsertDistribute(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//״̬�ж�
	ASSERT(pIServerUserItem->GetUserStatus()<US_PLAYING);
	if (pIServerUserItem->GetUserStatus()>=US_PLAYING) return false;

	bool bReturn=false;
	if(m_pIGameMatchServiceManager!=NULL) bReturn=m_pIGameMatchServiceManager->OnUserJoinGame(pIServerUserItem,0);
	return bReturn;
}

//���дʹ���
void CAttemperEngineSink::SensitiveWordFilter(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen)
{
	m_WordsFilter.Filtrate(pMsg,pszFiltered,nMaxLen);
}

//�����û�
bool CAttemperEngineSink::InsertWaitDistribute(IServerUserItem * pIServerUserItem)
{
	//����
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

	//����
	if(bAdd) m_WaitDistributeList.AddTail(pIServerUserItem);

	//֪ͨ
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_WAIT_DISTRIBUTE,NULL,0);

	return bAdd;
}

//ɾ���û�
bool CAttemperEngineSink::DeleteWaitDistribute(IServerUserItem * pIServerUserItem)
{
	//����
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

//�����û�
bool CAttemperEngineSink::DistributeUserGame()
{
	//�û����ж�
	WORD wNeedUserCount=__max(m_pGameServiceAttrib->wChairCount, m_pGameServiceOption->wMinDistributeUser);
	if(m_pGameServiceAttrib->cbDynamicJoin==TRUE)wNeedUserCount=2;
	if(m_WaitDistributeList.GetCount()<wNeedUserCount)return true;

	//�������
	for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if ((pTableFrame->IsGameStarted()==true)) continue;

		//����״��
		tagTableUserInfo TableUserInfo;
		WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

		//�����ж�
		if (wUserSitCount!=0) continue;

		//�����ж�
		if (m_WaitDistributeList.GetCount()>=TableUserInfo.wMinUserCount)
		{
			//��������
			WORD wHandleCount=0;
			WORD wWantUserCount=TableUserInfo.wMinUserCount;
			WORD wFreeUserCount=(WORD)m_WaitDistributeList.GetCount();

			//���ݵ���
			if (m_pGameServiceAttrib->wChairCount>TableUserInfo.wMinUserCount)
			{
				WORD wChairCount=m_pGameServiceAttrib->wChairCount;
				WORD wOffUserCount=__min(wChairCount,wFreeUserCount)-TableUserInfo.wMinUserCount;
				wWantUserCount+=(wOffUserCount > 0) ? (rand()%wOffUserCount) : 0;
			}

			//���´���
			POSITION pos=m_WaitDistributeList.GetHeadPosition();
			while(pos!=NULL)
			{
				//��������
				WORD wChairID=pTableFrame->GetRandNullChairID();

				//��Ч����
				if (wChairID==INVALID_CHAIR) continue;

				//���´���
				POSITION tempPos=pos;
				IServerUserItem *pUserItem = m_WaitDistributeList.GetNext(pos);
				if(pUserItem && pUserItem->GetUserStatus()==US_FREE && (pTableFrame->PerformSitDownAction(wChairID,pUserItem)==true))
				{
					//ɾ���Ŷ�
					m_WaitDistributeList.RemoveAt(tempPos);

					//���ñ���
					wHandleCount++;

					//����ж�
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

//���ò���
void CAttemperEngineSink::SetMobileUserParameter(IServerUserItem * pIServerUserItem,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	if(wPageTableCount > m_pGameServiceOption->wTableCount)wPageTableCount=m_pGameServiceOption->wTableCount;
	pIServerUserItem->SetMobileUserDeskCount(wPageTableCount);
	pIServerUserItem->SetMobileUserRule(wBehaviorFlags);
}

//Ⱥ������
bool CAttemperEngineSink::SendDataBatchToMobileUser(WORD wCmdTable, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//ö���û�
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//�����û�
		IServerUserItem *pIServerUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIServerUserItem==NULL) continue;
		if(!pIServerUserItem->IsMobileUser()) continue;

		//�������
		WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
		WORD wTagerTableID = pIServerUserItem->GetTableID();
		bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
		bool bRecviceGameChat = ((wMobileUserRule&RECVICE_GAME_CHAT)!=0);
		bool bRecviceRoomChat = ((wMobileUserRule&RECVICE_ROOM_CHAT)!=0);
		bool bRecviceRoomWhisper = ((wMobileUserRule&RECVICE_ROOM_WHISPER)!=0);

		//״̬����
		if(pIServerUserItem->GetUserStatus() >= US_SIT)
		{
			if(wTagerTableID != wCmdTable)continue;
		}

		//�������
		if(wSubCmdID==SUB_GR_USER_CHAT || wSubCmdID==SUB_GR_USER_EXPRESSION)
		{
			if(!bRecviceGameChat || !bRecviceRoomChat) continue;
		}
		if(wSubCmdID==SUB_GR_WISPER_CHAT || wSubCmdID==SUB_GR_WISPER_EXPRESSION)
		{
			if(!bRecviceRoomWhisper) continue;
		}

		//���ֿ���
		if(!bViewModeAll)
		{
			//��Ϣ����
			if(wSubCmdID==SUB_GR_USER_ENTER && wCmdTable==INVALID_TABLE) continue;
			if(wSubCmdID==SUB_GR_USER_SCORE && pIServerUserItem->GetUserStatus() < US_SIT) continue;
			
			//�������
			WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
			WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();

			//״̬��Ϣ����
			if(wCmdTable < wTagerDeskPos) continue;
			if(wCmdTable > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//������Ϣ
		SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//Ⱥ���û���Ϣ
bool CAttemperEngineSink::SendUserInfoPacketBatchToMobileUser(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));

	//�û�����
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//�û�����
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

	//�û�״̬
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//�û�����
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;

	//�û��ɼ�
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
	pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

	//������Ϣ
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

	//��������
	WORD wHeadSize=sizeof(tagMobileUserInfoHead);
	SendDataBatchToMobileUser(pUserInfoHead->wTableID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());

	return true;
}

//�������û���Ϣ
bool CAttemperEngineSink::SendViewTableUserInfoPacketToMobileUser(IServerUserItem * pIServerUserItem,DWORD dwUserIDReq)
{
	//��������
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
	WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
	WORD wTagerTableID = pIServerUserItem->GetTableID();
	WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
	WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();
	bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
	if(wTagerDeskCount==0) wTagerDeskCount=1;

	//ö���û�
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//�����û�
		IServerUserItem *pIUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIUserItem==NULL || (dwUserIDReq==INVALID_CHAIR && pIUserItem==pIServerUserItem)) continue;
		if(dwUserIDReq != INVALID_CHAIR && pIUserItem->GetUserID() != dwUserIDReq) continue;

		//���ֿ���
		if(dwUserIDReq==INVALID_CHAIR && !bViewModeAll)
		{
			if(pIUserItem->GetTableID() < wTagerDeskPos) continue;
			if(pIUserItem->GetTableID() > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//�������
		tagUserInfo * pUserInfo=pIUserItem->GetUserInfo();
		ZeroMemory(cbBuffer,sizeof(cbBuffer));
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));

		//�û�����
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//�û�����
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//�û�״̬
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//�û�����
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//�û��ɼ�
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIUserItem->GetFrozenedScore();

		//������Ϣ
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//��������
		WORD wHeadSize=sizeof(tagMobileUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//�ֻ�������¼
bool CAttemperEngineSink::MobileUserImmediately(IServerUserItem * pIServerUserItem)
{
	//��������
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame->IsGameStarted()==true) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//��Ч����
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;

		//�û�����
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//ʧ��
	m_TableFrameArray[0]->SendRequestFailure(pIServerUserItem,TEXT("û�ҵ��ɽ������Ϸ����"),REQUEST_FAILURE_NORMAL);
	return true;
}

//����ϵͳ��Ϣ
bool CAttemperEngineSink::SendSystemMessage(CMD_GR_SendMessage * pSendMessage, WORD wDataSize)
{
	//��Ϣ����
	ASSERT(pSendMessage!=NULL);

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength);
	if (wDataSize!=sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength) 
		return false;

	//���з���
	if(pSendMessage->cbAllRoom == TRUE)
	{
		pSendMessage->cbAllRoom=FALSE;
		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_SYSTEM_MESSAGE,pSendMessage,wDataSize);
	}
	else
	{
		//����ϵͳ��Ϣ
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

//�����Ϣ����
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

//����ֵ
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

//�洢�û���Ʒ
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

//��Ϸ��¼Json��ʽ
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

//��������
void CAttemperEngineSink::ShowRealOnlineCount()
{
	CString sOnline=TEXT("");
	sOnline.Format(TEXT("��ǰ����������%d��������ʵ��ң�%d��PID=%d"),m_ServerUserManager.GetUserItemCount(),m_ServerUserManager.GetRealUserItemCount(),_getpid());
	CTraceService::TraceString(sOnline,TraceLevel_Info);

	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (pTableFrame == NULL) continue;
		pTableFrame->getRecordCount();
	}
}

//д�ֲ��޳�
void CAttemperEngineSink::SaveAndKick()
{
	CMD_GR_KickAllUser KickAllUser={0};

	_sntprintf(KickAllUser.szKickMessage,CountArray(KickAllUser.szKickMessage),TEXT("��������ʱά�������ǽ���Ϊ�������÷�����������ɵĲ��㾡���½⣬���Ժ���뱾������������������Ϸ��"));

	//��ɢ������Ϸ
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		//��ȡ����
		CTableFrame * pTableFrame=m_TableFrameArray[i];
		if (!pTableFrame->IsGameStarted()) continue;
		pTableFrame->DismissGame();
	}

	tagBindParameter * pBindParameter=m_pNormalParameter;
	for (INT i=0; i<m_pGameServiceOption->wMaxPlayer; i++)
	{
		//Ŀ¼�û�
		IServerUserItem * pITargerUserItem=pBindParameter->pIServerUserItem;
		if (pITargerUserItem==NULL) 
		{
			pBindParameter++;
			continue;
		}
		//������Ϣ
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

//Ӯ��������Ƭ
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

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_USER_WIN_DRAGON_PIECE,pBindParameter->dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

	return true;
}

//Ӯ������
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

	//Ͷ������
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_WIN_PEARL,pBindParameter->dwSocketID,&WinPearl1,sizeof(WinPearl1));

	return true;
}

//���ܴ���
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

//ϵͳ���ͼ���
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

//������������
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

//���ͱ�������
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

//����ȫ���㲥
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

//��ȡ�û�����
void CAttemperEngineSink::GetUserItemCount(IServerUserItem * pIServerUserItem, int nType)
{
printf("%s %d\n", __FUNCTION__, __LINE__);
	DBR_GetUserItemCount GetUserItemCount1 = {};
	GetUserItemCount1.dwUserID = pIServerUserItem->GetUserID();
	GetUserItemCount1.nType = nType;

	tagBindParameter * pBindParameter = GetBindParameter(pIServerUserItem->GetBindIndex());
	m_pIDBCorrespondManager->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GET_USER_ITEM_COUNT,pBindParameter->dwSocketID,&GetUserItemCount1,sizeof(GetUserItemCount1));
}


//�����û�����
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
		TEXT("��ϲ��Vip%d%s���ڳ���������Ӯ��%ld��󽱡�"),pIServerUserItem->GetVipLevel(),pIServerUserItem->GetNickName(),nScore);
	//CTraceService::TraceString(stFakeAnnounce.szClownFakeAnnounceMent,TraceLevel_Normal);

	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_MANAGE,SUB_GR_CLOWN_FAKEANNOUNCEMENT,(VOID *)&stFakeAnnounce,sizeof(CMD_GR_CLOWNFAKEANNOUNCE),BG_COMPUTER);	
	CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
	if (pIServerUserItem->GetVipLevel()>0)
	{
		_sntprintf(VIPUpgrade.szTrumpetContent,CountArray(VIPUpgrade.szTrumpetContent),TEXT("#ffffff��ϲ��#ff0000Vip%d%s#ffffff���ڳ���������Ӯ��#ff0000%d��#ffffff�󽱣�#L01����˴�Ӯ�󽱣�"),pIServerUserItem->GetVipLevel(),pIServerUserItem->GetNickName(),nScore);
	}
	else
	{
		_sntprintf(VIPUpgrade.szTrumpetContent,CountArray(VIPUpgrade.szTrumpetContent),TEXT("#ffffff��ϲ��#ff0000%s#ffffff���ڳ���������Ӯ��#ff0000%d��#ffffff�󽱣�#L01����˴�Ӯ�󽱣�"),pIServerUserItem->GetNickName(),nScore);
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
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GRR_MatchSignUP));
	if (wDataSize!=sizeof(CMD_GRR_MatchSignUP)) return false;

	CMD_GRR_MatchSignUP * pUserMatchSignUP = (CMD_GRR_MatchSignUP *)pData;
	//CMD_GRO_MatchSignupRes stUserMatchSignUpRes = {0};
	//if (m_bMatchSignUpMark == false)
	//{
	//	//����ʱ����δ��ʼ ������ʾ��Ϣ
	//	stUserMatchSignUpRes.dwUserID = pUserMatchSignUP->dwUserID;
	//	stUserMatchSignUpRes.bIsSignUpSuc = false;
	//	lstrcpyn(stUserMatchSignUpRes.szDescribeString,TEXT("����ʱ����δ��ʼ"),CountArray(stUserMatchSignUpRes.szDescribeString));

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
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GPR_MatchGetMatchPrise));
	if (wDataSize!=sizeof(CMD_GPR_MatchGetMatchPrise)) return false;

	CMD_GPR_MatchGetMatchPrise * pGetMatchPrise = (CMD_GPR_MatchGetMatchPrise *)pData;
	DBR_GR_Match_Get_Prize stMatchGetPrize={0};
	stMatchGetPrize.dwUserID = pGetMatchPrise->dwUserID;
	stMatchGetPrize.nMatchID = pGetMatchPrise->nMatchID;
	lstrcpyn(stMatchGetPrize.szMatchData,pGetMatchPrise->szMatchData,sizeof(stMatchGetPrize.szMatchData));
	printf("���id=%d,��������=%d,��������=%s\n",stMatchGetPrize.dwUserID,stMatchGetPrize.nMatchID,stMatchGetPrize.szMatchData);
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
