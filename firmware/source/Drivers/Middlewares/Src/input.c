#include "input.h"
struct PinyinItem{
    const char* py;
    const char* chars;
};
static const struct PinyinItem py_table[]={
    {"a","°¡°¢ºÇëç"},
    {"ai","°®°¬°£°¥°­°¦°©°§°¤°«°ª°¨ŞßêÓ"},
    {"an","°²°¸°µ°´°¶°³°±°·°°÷öğÆ"},
    {"ang","°º°¹°»óa"},
    {"ao","°Â°Á°¾°Äà»°¼°½°Ã°¿Ûê"},
    {"ba","°É°Ñ°Ö°Í°Ë°Õ°Î°Å°Ô°Ê°Ì°È°Ó°Ç°Ğ°Ï°Ò"},
    {"bai","°×°İ°Ù²®°Ü°Ú°Øêş°Û"},
    {"ban","°ì°à°ë°å°á°é°ã°ç°æ°ê°ß°â°ä°í°èÚæÛà"},
    {"bang","°ï°ô°î°ó°ò°õ°÷°ñ°ø°ù"},
    {"bao","±£±§±¨±¦°ü±©±¬±¤±«ÅÚ±¡°û±¥±ª°şÅÙñÙìÒ°ú"},
    {"bei","±»±¸±´±­±³±±±¯±²±¶±Û±°ßÂ±®±µ±¹İí±·±º"},
    {"ben","±¾±¿±¼±½"},
    {"beng","±À±Ä±Á±Â±ÃàÔÜ¡"},
    {"bi","±È±Ø±ÕÃØ±Ë±Ê±Ï±Æ±Ü±İ±Ç±Ú±Û±Ò±Ğ±É±Ó±×±Ì±ÎßÙØ°ÃÚ±ÔÙÂ±Ù±Ñ"},
    {"bian","±ß±ä±ã±é±à±ç±æ±âòù±Ş±è±á"},
    {"biao","±í±êæ»ïÚì­±ëïğ"},
    {"bie","±ğ±ï±ñõ¿"},
    {"bin","±öéÄ±õ±ò±ôéë±óë÷"},
    {"bing","²¢²¡±ø±ù±ıéÄÆÁ±ş±úÙ÷±ûŞğ"},
    {"bo","°Ù²®²¨²©²¥²ª±¡°Ø²£²±²¦²·²«²²°ş²´²µ²¤õËŞ¬ô¤²°²§"},
    {"bu","²»²¿²¼²½²¶²À±¤²¹²·²¾²¸ÆÒîĞ"},
    {"ca","²Á"},
    {"cai","²Å²Â²Ë²É²Ê²Æ²Ä²Ã²È²Ì²Ç"},
    {"can","²Í²Î²Ğ²Ò²Ó²Ñ²Ï"},
    {"cang","²Ø²Õ²Ö²Ô²×"},
    {"cao","²Ù²İ²Û²Ü²ÚÜ³àĞ"},
    {"ce","²â²ß²Ş²à²á"},
    {"cen","²Î"},
    {"ceng","Ôø²ã²ä"},
    {"cha","²é²ì²î²è²å²æÉ²²íÔû²ç²ï²ë²ê"},
    {"chai","²î²ğ²ñ²ò"},
    {"chan","µ¥²ú²ø²üâã²ù²ô²ö²ûìøæ¿"},
    {"chang","³¤³£³¡³ª³¢³§³¥³¦³¨³©æ½²ı³«"},
    {"chao","³¬³³³¯³±³´³°³­³²³®´Â½Ë"},
    {"che","³µ³¶³·³¹³º"},
    {"chen","³Æ³Á³¿³Â³Ã³¾³Ä³¼³½³À"},
    {"cheng","³É³Ç³Ì³Ğ³Æ³ÏÊ¢³Ë³Å³Í³Ê³È³Ñ³Îáç³Óîõ³ÒØ©"},
    {"chi","³Ô³Ö³Ù³×³Õ³ß³Ø³Ü³İ³á³à³âÛæ³Û³Şñİ³ã"},
    {"chong","ÖÖÖØ³å³ä³æ³ç³èã¿âç"},
    {"chou","³é³ô³ğ³ó³ï³ê³î³ñ³ò³ì³í"},
    {"chu","³ö´¦³ı³ş³õ³ø´¥´¢Ğó´¡³÷´£´¤³û³ùèÆ"},
    {"chuai","´§õß"},
    {"chuan","´©´¬´«´­´¨´®"},
    {"chuang","´²´´´°´³´±´¯"},
    {"chui","´µ´¹´¸´¶é³´·"},
    {"chun","´À´¿´º´½´¼´»´¾ğÈ"},
    {"chuo","´Á´Âöºà¨ê¡"},
    {"ci","´Î´Ë²î´Ì´Ê×È´Ç´È´Í´Å´Ä´ÆËÅ´É´Ã"},
    {"cong","´Ó´Ï´Ô´Ò´Ğ´Ñ"},
    {"cou","´Õ"},
    {"cu","´Ö´Ù´×"},
    {"cuan","´Ü´ÛÔÜ´Ú"},
    {"cui","´à´İ´Ş´â´ß´äã²İÍ"},
    {"cun","´æ´å´ç"},
    {"cuo","´í´ë´ì´é´ê´è"},
    {"da","´ó´ò´ï´ğ´îßÕ´ñàªæ§"},
    {"dai","´ó´ø´ı´ú´÷´ô´ü´ş÷ì´û´õµ¡´ù´ö"},
    {"dan","µ«µ°µ¥µ£µ¯Ê¯µ¤µ®µ©µ¨µ­µ¢µªíñÉÄµ§"},
    {"dang","µ±µµµ³µ²µ´îõñÉ"},
    {"dao","µ½µÀµ¹µ¼µ¶µºµ»µÁµ¸µ·ß¶µ¿µ¾"},
    {"de","µÄµÃµØµÂ"},
    {"dei","µÃ"},
    {"deng","µÈµÇµÆµËµÉ³ÎµÊµÅ"},
    {"di","µÄµØµÚÌáµÜµ×µÛµÏµÙµÍµĞµÖµÎµİµÒµÑÛæµÌÛ¡àÖµŞíûæ·ÚĞÚ®µÓ"},
    {"dia","àÇ"},
    {"dian","µãµçµêµäµîµéµæµßñ²çèµìµàµëáÛµá"},
    {"diao","µôµ÷µõµöµñµóõõµïµğµò"},
    {"die","µùµøµıµúµûµşà©µüñóëº"},
    {"ding","¶¨¶¡¶¥¶©¶¢¶¤¶£¶¦î®ôú¶§"},
    {"diu","¶ª"},
    {"dong","¶¯¶«¶®¶´¶¬¶³¶­¶°ßËíÏ"},
    {"dou","¶¼¶·¶¹¶º¶¶¶µ¶»¶¸"},
    {"du","¶¼¶È¶¾¶À¶Á¶Ä¶½¶Å¶Ç¶É¶Ê¶Â¶Ãà½äÂ¶Æ"},
    {"duan","¶Ï¶Î¶Ì¶Ë¶Í"},
    {"dui","¶Ô¶Ó¶Ñ¶Ò"},
    {"dun","¶Ù¶Ü¶Ø¶×¶ÖìÀ¶Ûíïãç¶Õ¶Ú¶İ"},
    {"duo","¶à¶È¶ã¶ä¶á¶é¶æ¶â¶å¶ç¶ß¶èßÍîì"},
    {"e","Å¶°¢¶ñßÀ¶ö¶í¶î¶ò¶ìØ¬öù¶ó¶ğ¶ô¶ê¶ëò¦"},
    {"ei","ÚÀ"},
    {"en","¶÷Şô"},
    {"er","¶ù¶ø¶û¶ş¶ú¶ı¶ü·¡"},
    {"fa","·¢·¨·£·¦·¥·§·¤"},
    {"fan","·´·¸·³·¹·­·¶·²·µ···¬·±·«·ºèó·ªŞ¬"},
    {"fang","·½·Å·¿·À·Ã·Â·Á·¼·»·¾·Ä"},
    {"fei","·Ç·É·Ñ·Æ·È·Ï·Ê·Ë·Î·Ğåúì³·Íáôìé·ÌôäòãíÉëèç³"},
    {"fen","·Ö·İ·Û·Ü·Ò·ß·Ø·Õ·à·Ô·×·Ù·Ó"},
    {"feng","·è·ç·â·î·ä·í·ì·á·æ·å·ï·ê·ë·ã"},
    {"fo","·ğ"},
    {"fou","·ñ"},
    {"fu","·ò·ş¸¸¸¶¸´¸£¸¥¸º·ğ¸®¸»¸½¸¾¸±·û·ü¸¯·ù·ô¸²¸¡¸§¸µ¸³·ö¸¹Ü½òğ·ı¸¨¸À·ø¸°¸«¸©¸¿·ó·õ¸¤¸¦·÷¸ª·ú¸¬íÉ¸­¸¼"},
    {"ga","¿§¼ĞŞÎÙ¤¸Â¸Á"},
    {"gai","¸Ã¸Ä¸Ç¸Å½æØ¤¸È"},
    {"gan","¸É¸Ğ¸Ò¸Ï¸Ê¸ËŞÏ¸ÓéÏ¸Î¸Ì¸Ñ¸Í"},
    {"gang","¸Õ¸Û¸Ö¸Ü¸×¸Ô¸Ú¿¸¸Ø"},
    {"gao","¸æ¸ß¸ã¸â¸å¸àØº¸á"},
    {"ge","¸ö¸çºÏ¸ñ¸÷¸è¸Ç¸ô¸ğ¸ï¸ê¸î¸ó¿©¸ò¸ì¸ë¸éàÃ¸í¸õ÷À"},
    {"gei","¸ø"},
    {"gen","¸ú¸ùôŞ"},
    {"geng","¸ü¾±¹¢¸û¹£¸ş"},
    {"gong","¹¤¹«¹¦¹²¹©¹¥¹§¹¬¹±¹­¹ª¹°¹®ò¼"},
    {"gou","¹»¹·¹¹¹º¹³¹´¹µ¹¶¹¸óô"},
    {"gu","¹Ê¹Ë¹Å¹É¹Ã¹Ç¹Â¹Í¹À¹È¹Ä¼Ö¹Ì¹¼¹¾¹½¹Æ¹ÁïÀ"},
    {"gua","¹Ò¹Ï¹Î¹ÑØÔßÉ¹Ğ"},
    {"guai","¹Ö¹Ô¹Õ"},
    {"guan","¹Ø¹Ü¹Ù¹Û¹İ¹Ú¹ß¹Ş¹à¹×¹áîÂ"},
    {"guang","¹â¹ã¹äë×"},
    {"gui","¹í¹æ¹ó¹é¹ï¹ñ¹ì¹ò¹ê¹å¹î¹ğ¹ë¹ç¹ôöÙ¹è"},
    {"gun","¹ö¹÷¹õ"},
    {"guo","¹ı¹û¹ú¹ü¹ø¹ùÎĞàş"},
    {"ha","¹ş¸ò"},
    {"hai","»¹º¢º¦º£àË¿Èº¡º¥º§"},
    {"han","º²ººº¶º°º¹º«º¬º®º±º¸º³º·º´º¯ºµº­÷ıº©å«"},
    {"hang","ĞĞº½Ïïº¼¿Ôº»"},
    {"hao","ºÃºÅºÁºÀºÄºÆº¿º¾ºÂ"},
    {"he","ºÍºÎºÏºÈÏÅºÓºÇºËºĞºÉºÕºØºÖàÀº×ºÊºÔ"},
    {"hei","ºÙºÚ"},
    {"hen","ºÜºŞºÛºİ"},
    {"heng","ºàºßºâºãºáçñ"},
    {"hong","ºìºäºéºçºåºëºèºæºê"},
    {"hou","ºóºòááºïºñºíºğºîåË"},
    {"hu","»¤ºõºôºú»§»¥»¡ºË»¢ºıºşºö»£ºûºüºøº÷çúã±ºù"},
    {"hua","»°»¨»¯»®»­»ª»¬»«»©í¹"},
    {"huai","»µ»³»²õ×»´"},
    {"huan","»¹»¶»»»·»Ã»¼»½»º»À»¾ä½"},
    {"huang","»Ñ»Ê»Æ»Ä»Å»Î»Í»Ë»É»Ï»Ç»Ğ»Ì»Èáå"},
    {"hui","»á»Ø»Ù»Ó»Ò»Ú»Ö»İ»Û»ãÀ£»Ô»æ»Õ»ß»Şåç»à»â»åà¹Üî"},
    {"hun","»é»ì»ê»è»ë"},
    {"huo","ºÍ»î»ò»ï»ğ»õ»ñ»ô»ó»öâ·»íàë"},
    {"ji","¸ø¼º»ú¼Ç¼¸¼ÆÏµÆæ»÷¼Ì»ù¼ª¼°¼¯¼«¼´¼¤¼±¼¶¼Í¼¦¼Ê¼¼¼£¼È¼Ë¼¾¼Ä»ø¼Ã¼É¼Á»ıí¶¼­¼·¼²¼µ¼Å¼§¼¡¼À¼¨¼©¼¢½å»ü¼¹¼®¼¬ß´¼¿»ûî¿á§ßó¼³¼»êª¼½"},
    {"jia","¼Ò¼Ó¼Ù¼Ü¼Û¼Ş¼İ¼×¼Ö¼Ñ¼Ğ¼ÎÇÑÙ¤¼Õ¼ÚåÈ¼Ïçì"},
    {"jian","¼û¼ä¼ş½¨¼ò¼à¼ì¼á½¡¼â¼õ½£½¢¼ô¼ü¼ú¼é¼ı¼ç½¥¼è¼ñ¼å¼ö¼ø¼æ¼ù½¦¼í¼ß¼ğ¼ë¼î¼ê"},
    {"jiang","½«Ç¿½²½µ½±½´½­½©½¬½³½ªºç½¯½°½®"},
    {"jiao","¾õ½Ğ½Ì½»Ğ£½Ç½Å½Ï½¾½¹½º½Á½·½Æ½¼õÓ½¶½Ê½À½Í½¸½Ñ½½½Î½¿½É½Ã½È½ÄÙ®½Ë"},
    {"jie","½Ó½â½á½ã½ç½Ü½Ú½é½Ö½è½ä½Ù½à½×½Ò½Ø½İ½å½ß½Ô½ì½æ½Ş½Õ½ë½Û"},
    {"jin","½ø½ñ½ğ½ü½ô¾¡½ö½û¾¢½í½ï½÷½î½ò½ş½ú½õ½ıÚá"},
    {"jing","¾­¾¯¾²¾«¾ª¾¹¾¦¾³¾µ¾´¾°¾»¾¢¾º¾©¾¶¾®¾¨¾§Úå¾±¾¥ö¦¾·¾£ã½İ¼¾¤¾¸"},
    {"jiong","¾½"},
    {"jiu","¾Í¾È¾Æ¾Ã¾¿¾Å¾É¾Ë¾À¾Î¾Ç¾¾¾Ì¾ÄğÕ¾Ê"},
    {"ju","³µ¾Ö¾İ¾ß¾Ó¾ç¾ä¾Ù¾Ş¾Û¾Ü¾å¾ã¾à¾Ø¹ñ¾Ğ¾Ú¾Ñ¾Õ¾âéÙ¾Ô¾Ïì«¾á¾×¾æÜÄ"},
    {"juan","È¦¾í¾è¾ë¾ì¾ê"},
    {"jue","¾õ¾ö¾ø½Ç½Å¾ô¾÷¾ò½À¾ñ¾ğáÈ¾óâ±ØÊ"},
    {"jun","¾ü¾ı¿¡¹ê¾ù¾ú¿¤¾ş¿¥"},
    {"ka","¿¨¿§¿©ßÇ¿¦"},
    {"kai","¿ª¿­¿®¿¬âı¿«"},
    {"kan","¿´¿³¿°¿²¿¯¿±î«"},
    {"kang","¿µ¿¹¿¶¿¸¿º"},
    {"kao","¿¼¿¿¿¾îí¿½"},
    {"ke","¿É¿Ë¿Í¿Æ¿Ì¿ÎºÇ¿Å¿Ê¿Â¿Ç¿Ã¿Èà¾¿Áçæ¿Äî§ë´"},
    {"ken","¿Ï¿Ò¿Ğ¿Ñ"},
    {"keng","¿Ó¿Ô"},
    {"kong","¿Õ¿Ø¿Ö¿×áÇ"},
    {"kou","¿Ú¿Û¿Ü¿Ùßµ"},
    {"ku","¿à¿Ş¿á¿â¿ã¿İ¿ß÷¼ç«"},
    {"kua","¿ä¿ç¿å¿è"},
    {"kuai","»á¿ì¿é¿ê"},
    {"kuan","¿î¿í"},
    {"kuang","¿ö¿ñ¿ó¿ò¿ï¿ô¿õ¿ğ"},
    {"kui","¿÷À£À¢¿ú¿ù¿ø¿ü¿ı¿û¿şÀ¡Ùç"},
    {"kun","À§À¥À¦À¤"},
    {"kuo","À¨À©À«Àª"},
    {"la","À­À²ÂäÀ¬À±À°À¯À®åå"},
    {"lai","À´À³Àµäşíù"},
    {"lan","À¼ÀÃÀ¶À¹ÀºÀ¸ÀÁé­ÀÄÀÀÀ·ÀÂÀ»À¿"},
    {"lang","ÀËÀÊÀÇÀÉÀÈòë"},
    {"lao","ÀÏÂäÀÍÀÎÂçÀĞÀÒÀÌßëÀÑÀÓ"},
    {"le","ÁËÀÖÀÕ"},
    {"lei","À×ÀàÀÕÀÛÀÙÀáÀİÀßàÏÀÜÀŞ"},
    {"leng","Àäã¶Àâ"},
    {"li","ÀïÀíÁ¦ÀûÀëÀöÀñÁ¢ÀòÀúÀîÀ÷ÀıÀèÁ§Á¥ÀøÁ¨Á£ÀêÀæÀõÀåáûö¨à¬ÀéòÃİ°ÀçÀğÀşÀô"},
    {"lia","Á©"},
    {"lian","ÁªÁ¬Á³Á·ÁµÁ¯Á®Á´Á«Á¶Á±Á²"},
    {"liang","Á½ÁÁÁ¿Á©ÁÂÁ¾Á¼Á¹ÁºÁ¸ÁÀ"},
    {"liao","ÁËÁÏÁÄÁÆÁÅÁÉÁÃÁÎÁÌÁÊ"},
    {"lie","ÁĞÁÒÁÔÁÑÁÓßÖ÷à"},
    {"lin","ÁÖÁÙÁÕÁÚÁÜÁàÁßÁİ÷ëÁ×õïÁÛÁŞ"},
    {"ling","ÁîÁíÁìÁéÁãÁåÁäÁèÁáñöÁçÁêÁëÀâ"},
    {"liu","ÁôÁ÷ÁùÂ½ÁïÁõÁöÁøÁñÂµÁòåŞä¯ÁóÁğ"},
    {"lo","¿©"},
    {"long","ÅªÁúÂ¡ÁıÁüÁûÂ£ëÊÂ¢Áş"},
    {"lou","Â¥Â¶Â©à¶ÂªÂ§÷ÃÂ¦"},
    {"lu","Â·ÁùÂ¼Â¶Â³Â½Â¬ÂÌÂ¹Â¯Â¾Â²ààÂ¸ÂµÂ­Â«öÔÂ°Â»ê¤ß£è´"},
    {"lv","ÂÉÂÃÂÇÂÊÂÌÂÂÂ¿ÂÄÂÀÂËÂÅéµÂÈÂÁÂÆ"},
    {"luan","ÂÒÂÑÂÎÂÏ"},
    {"lue","ÂÔÂÓ"},
    {"lun","ÂÛÂ×ÂÖÂØÂÙ"},
    {"luo","ÂŞÂäÂåÂçÂÜ¿©ÂãÂßÂİÂæÀÓÂâÂà¸õâ¤"},
    {"m","ß¼"},
    {"ma","ÂğÂèÂíÂïÂéÂêÂëÄ¦ÂîÄ¨æÖÂìó¡"},
    {"mai","ÂúÂıÂüÂşÂñÂ÷ÂùÂûÂø÷©"},
    {"man","ÂúÂıÂüÂşÂ÷ÂùÂûÂø"},
    {"mang","Ã¦Ã¤Ã¥Ã¢Ã£Ã§òş"},
    {"mao","Ã«Ã°Ã¨Ã±Ã²Ã¬Ã³÷ÖÃªÃ¯Ã©"},
    {"me","Ã´"},
    {"mei","Ã»ÃÀÃ¿ÃÃÃ·Ã½Ã¹Ãµ÷ÈÃ¶Ã¼İ®ÃºÃÁÃÄÃÂé¹Ã¸"},
    {"men","ÃÇÃÅÃÆŞÑ"},
    {"meng","ÃÎÃÉÃËÃÍÃÏÃÊÃÈëü"},
    {"mi","Ã×ÃÜÃØÃÔÃÛßäÃÖÃÕÃÒÃÑÃÙÃÚ÷ç"},
    {"mian","ÃæÃâÃßÃäÃàÃŞÃãÃåÃá"},
    {"miao","ÃîÃëÃèÃéÃíÃçÃìß÷çÑÃê"},
    {"mie","ÃğßãÃïØ¿"},
    {"min","ÃñÃôÃöÃõãÉ"},
    {"ming","Ã÷ÃûÃüÃùÃúÚ¤Ãø"},
    {"miu","ÃıçÑ"},
    {"mo","Ã»ÍòÄ§ÄªÄ£Ä¦÷áÄ©Ä¬ÃşÄ¥Ä«Ä°Ä®ÂöÄ¨Ä¯Ä¤Ä­ÜÔÄ¢"},
    {"mou","Ä³Ä±çÑÄ²íøßè"},
    {"mu","Ä¿Ä¸Ä·Ä»Ä£Ä¾ÄÁÄ¹ÄÂÄ½ÄµÄ¼Ä´ãåÄ¶ÄÀÄºÄ²"},
    {"na","ÄÇÄÄÄÃÄÈÄÉÄÅÄÆ"},
    {"nai","ÄÌÄÍÄÎÄËÙ¦"},
    {"nan","ÄĞÄÑÄÏà«éªàï"},
    {"nang","ÄÒ"},
    {"nao","ÄÔÄÖÄÕÄÓØ«è§"},
    {"ne","ÄØÄÄ"},
    {"nei","ÄÚ"},
    {"nen","ÄÛ"},
    {"neng","ÄÜ"},
    {"ng","àÅ"},
    {"ni","ÄãÄØÄáÄİÄàÄäÄâÄæÄçÄåêÇÄß"},
    {"nian","ÄêÄîğ¤ÄëØ¥Äìöó"},
    {"niang","ÄïÄğ"},
    {"niao","ÄñÄò"},
    {"nie","ÄõÄóÄùØ¿ÄøÄô"},
    {"nin","Äú"},
    {"ning","ÄşÄıÄûÅ¡Å¢"},
    {"niu","Å£Å¦æ¤Å¥Å¤"},
    {"nong","ÅªÅ©Å¨Ù¯Å§ßæ"},
    {"nou","ññ"},
    {"nu","Å¬Å­Å«åó"},
    {"nuan","Å¯"},
    {"nue","Å°"},
    {"nuo","ÄÈÅµÅ³Å²ßö"},
    {"nv","Å®ô¬"},
    {"o","Å¶àŞà¸"},
    {"ou","ÇøÅ·Å¼Å»Å¹Å¸"},
    {"pa","ÅÂÅÁÅÀÅ¿°ÇÅ¾İâ°Ò"},
    {"pai","ÅÉÅÄÅÅÅÆÆÈÅÇ"},
    {"pan","ÅĞÅÌÅÑÅÖ·¬ÅËÅÎÅÊŞÕÅÏÅÍ"},
    {"pang","ÅÔÅÖ°ò°õ°÷ÅÓó¦ÅÒáİ"},
    {"pao","ÅÜÅİÅÚÅ×ÅÛÅØÅÙğå"},
    {"pei","ÅãÅäÅåÅàÅâÅáÅŞÅæÅß"},
    {"pen","ÅçÅè"},
    {"peng","ÅóÅöÅéÅñÅíÅëÅïÅõÅîÅòÅôàØÜ¡Åê"},
    {"pi","·ñÆ¤Æ¨Æ¡ÅúÆ¥ÅıÅûÆ¢Æ£ñ±ÅüÆ§ÅùÆ¦Æ©±ÙæÇ"},
    {"pian","Æ¬±ãÆ­Æ«Æª±âôæ"},
    {"piao","Æ¯Æ±Æ®ÆÓæÎÆ°"},
    {"pie","Æ²Æ³ÜÖ"},
    {"pin","Æ·ÆµÆ´Æ¶Æ¸"},
    {"ping","Æ½Æ¿ÆÀÆ¾Æ»ÆÁ·ëÆºÆ¹Æ¼"},
    {"po","ÆÆÆÅÆÈçê·±ÆÂÆÓ²´ÆÃÆÄÆÇ"},
    {"pou","ÆÊ"},
    {"pu","ÆÕ±©±¤ÆÏÆÍÆÌÆ×ÆËÆÓÆÙÆØÆÖÆĞÆÒàÛ¸¬ÆÑ"},
    {"qi","ÆğÆäÆøÆæÆÚÆ÷ÆŞÆúÆßÆûÆïÆôÆÛÆíÆóÆëç÷ÆìÆüÆå¼©ÆõÆáÆİÆçä¿Æò»üÆñÆéÆÜí¬ÆùçùÆàÆêõè÷¢Æö÷èÆã"},
    {"qia","¿¨Ç¡ÆşÇ¢"},
    {"qian","Ç°Ç®Ç¸Ç§Ç©Ç·Ç±Ç£Ç¬ÜçÇ²Ç³Ç¨Ç«Ç¦ò¯Ç´ÏËÇ¯Ç¶Ù»"},
    {"qiang","Ç¹Ç¿ÇÀÇ½Ç»Çº"},
    {"qiao","ÇÇÇÆÇÉÇÃÇÅÇÄ¿ÇÈ¸ÇÌÇÏÇËÇÁÇÎõÎã¾ÇÍÇÈ"},
    {"qie","ÇĞÇÒÇÔÇÑÙ¤ÇÓæªã«"},
    {"qin","Ç×ÇÙÇÖÇÚÇİÇÜÇÕÇØÇÛÇŞ"},
    {"qing","ÇéÇëÇ×ÇåÇáÇàÇìÇãÇæÇçÇäÇâòß"},
    {"qiong","ÇîÇíñ·"},
    {"qiu","ÇóÇò³ğÇïÇô¹êÇğôÃÇõÇñôÜ"},
    {"qu","È¥È¡È¤ÇøÇúÈ¢ÇıÇüÇûÇşÇ÷ÇùÜÄ"},
    {"quan","È«È¨È¦È­È°ÈªÈ®È¯È¬Ú¹òé"},
    {"que","È·È´È±È¸È³ãÚ"},
    {"qui","÷ü"},
    {"qun","ÈºÈ¹"},
    {"ran","È»È¾È¼È½"},
    {"rang","ÈÃÈÂÈÀ"},
    {"rao","ÈÅÈÆÈÄ"},
    {"re","ÈÈÈÇßö"},
    {"ren","ÈËÈÏÈÎÈÌÈÊâ¿ÈĞÈÍÈÒ"},
    {"reng","ÈÔÈÓ"},
    {"ri","ÈÕ"},
    {"rong","ÈİÈÙÈÚÈÜÈŞÈÛÈØÈ×"},
    {"rou","ÈâÈáÈàõå"},
    {"ru","ÈçÈëÈèÈéÈåÈêÈäÈã"},
    {"ruan","ÈíÈî"},
    {"rui","ÈğÈñÜÇÈïî£"},
    {"run","Èó"},
    {"ruo","ÈôÈõ"},
    {"sa","ÈøÈöÈ÷Øí"},
    {"sai","ÈüÈû"},
    {"san","ÈıÉ¢É¡Èş"},
    {"sang","É£É¥É¤"},
    {"sao","É¨É§É©É¦ëı"},
    {"se","É«ÈûÉªÉ¬ØÄ"},
    {"sen","É­"},
    {"seng","É®"},
    {"sha","É±ÉµÉ¯É³É¶É¼öèÉ´ÏÃÉ²É°É·"},
    {"shai","É«É¹É¸"},
    {"shan","µ¥É½ÉÆÉÁÉºÉ¼ÉÃÉÀÉÈÉ¾²ôÉ¿Õ¤Ú¨ìøÉÄÉÅ"},
    {"shang","ÉÏÉËÉÌÉÍÉĞÉÑ"},
    {"shao","ÉÙÉÕÉÔÉÜÉÚÉ×ÉÓÉÒ"},
    {"she","ÉäÉèÉçÉãÕÛÉßÉæÉáÉàÉâÉİÉŞ"},
    {"shei","Ë­"},
    {"shen","Ê²ÉíÉñ²ÎÉõÉîÉóÉêÉìÉ÷ÉğÉôÉëÉöÉøÉò"},
    {"sheng","ÉúÉùÊ¥ÉıÊ£Ê¤Ê¢³ËÉüÊ¡ÉşÉûêÉ"},
    {"shi","ÊÇÊ²ÊÂÊ±ÊµÊ¿Ê¼Ê§ÊÀÊ®ÊÔÊ¦Ê¹Ê¶ÊÒÊÓÊ½Ê·ÊĞÊ¾Ê³ËÆÊ¯ÊÍÊÊÊ¬ÊÄ³×ÊºÊ©ÊÆÊ°Ê«Ê»ÊªÊÎÊ¨ÊÏÊÅÖ³ÊÌÊÈÊÉÊ´ÊÃÊ¸Ê­ÊÁÊËÖÅ"},
    {"shou","ÊÖÊÜÊÕÊ×ÊØÊÚÊŞÊÛÊİÊÙá÷"},
    {"shu","ÊıÊéÊøÊõÊåÊ÷ÊôÊäÊìÊæÊóÊöÊâÊğË¡ÊíÊêÊèÊáÊîÊûÊßÊçÊúïíÊïÊşÊãÊà"},
    {"shua","Ë£Ë¢"},
    {"shuai","Ë§ÂÊË¤Ë¦Ë¥ó°"},
    {"shuan","Ë¨Ë©"},
    {"shuang","Ë«Ë¬Ëªæ×"},
    {"shui","ËµË®Ë¯Ë°"},
    {"shun","Ë³Ë²Ë±"},
    {"shuo","ËµÊıË¶Ë¸"},
    {"si","ËÀË¹Ë¼ËÄË¾Ë¿ËÆË½ËºËÂËÁËÅËÇØËËÃË»ãáìë"},
    {"song","ËÍËÉËÎËÏËÌã¤ËĞËÊËË"},
    {"sou","ËÑËÒËÔâÈ"},
    {"su","ËßËÙËÕËØöÕËõËŞË×ËÜËàËÖËİËÚ"},
    {"suan","ËãËáËâ"},
    {"sui","ËæËêËäËéËíËîËèËìËë"},
    {"sun","ËğËïËñ"},
    {"suo","ËùË÷É¯ËøËõËóàÂËöËô"},
    {"ta","ËûËıËüËşÌ¤ËúÍØÌ£åİÌ¢"},
    {"tai","Ì«Ì¨Ì¬Ì©Ì¥Ì§Ì­Ì¦õÌîÑ"},
    {"tan","Ì¸µ¯Ì½Ì¹Ì²ÌºÌ¾Ì°Ì¯Ì³Ì¼Ì·Ì±Ì¿Ì»ÌµÌ®Ì¶"},
    {"tang","ÌÃÌÀÌÇÌÆÌÉÌËÌÌÌÅÌÁÌÊîõÌÈó«"},
    {"tao","ÌÓÌÖÌ×ÌÒÌÑÌÔÌÕß¶ÌÍÌÏÌÎ"},
    {"te","ÌØß¯"},
    {"teng","ÌÛÌÙÌÚëø"},
    {"ti","ÌâÌåÌáÌæÌİÌßÌêÌëÌèÌŞÌéÌãÌäÌçç¾"},
    {"tian","ÌìÌğÌïÌîÌíÌò"},
    {"tiao","ÌõÌøµ÷ÌôÌö"},
    {"tie","ÌúÌùÌû"},
    {"ting","ÌıÍ£Í¥Í¦ÌüÍ§Í¡Í¢æÃöªÍ¤òÑ"},
    {"tong","Í¬Í³Í´Í¯Í°Í²Í±Í­Í«Íª"},
    {"tou","Í·ÍµÍ¶Í¸÷»"},
    {"tu","Í¼Í»ÍÁÍ½Í¾ÍÂÍÃÍ¿ÍÀÍºÍ¹"},
    {"tuan","ÍÅ"},
    {"tui","ÍËÍÆÍÈÍÊÍÇ"},
    {"tun","ÍÌëàÍÎÍÊ¶Ú"},
    {"tuo","ËµÍĞÍÑÍÏÍ×ÍÕÍÓÍØÛçÍÙÍÒ"},
    {"wa","ÍÛÍßÍŞÍÚÍàÍÜ"},
    {"wai","ÍâÍá"},
    {"wan","ÍíÍêÍæÍòÍèÍäÍëÍåÍìÍóÍçÂûÍğÍñÍãÍéæıÍï"},
    {"wang","ÍûÍõÍüÍùÍöÍøÍıÍúÍ÷Íôã¯"},
    {"wei","ÎªÎ»Î¹Î´Î¬ÍşÎ¶Î£Î¨ÎÀÎ§Î¢Î°Î½Î¾Î²Î¯Î¥Î¤Ş±Î±Î¿Î¸Î·ÎºÎ®â«ÎµÎ³Î©Î¦Î­â¬"},
    {"wen","ÎÊÎÄÎÅÎÂÎÇÎÈÎÆÎÁÎÃö©ÎÉ"},
    {"weng","ÎÌÎËÎÍ"},
    {"wo","ÎÒà¸ÎÕÎÖÎÔÎÑÎĞÎÏö»ÎÎÙÁ"},
    {"wu","ÎŞÎïÎåÎñ¶ñÎèÎçÎäÎİÎóì¶ÎéÎ×ÎÚßíÎÛÎíÎëÎêÎØÎòÎâÎğÎáğÄÎãÎæÎîÎÜÎßòÚ"},
    {"xi","Î÷Ï²Ï£ÏµÏ¢ÎüÏ·Ï´Ï°Ï¸Ï¯Ï§Ï®ÎşÏ¤ÎöÜçÏ¥Ï¡Ï¦ÙâÏ¨ÎúÎôÏªÏ±ÎıòáæÒÎõÏ¬Ï¶áãÎûó¬ßñõèôËÏ«êØ"},
    {"xia","ÏÂÏÅÏÄÏ¹ÏÀÏºÏÃÏ¿Ï½è¦ÏÁÏ¼Ï»Ï¾"},
    {"xian","ÏÖÏÈÏÕÏßÏ´ÏÔÏŞÏİÏÊÏÓÏ×ÏĞÏÉÏÚÏÜÏÒÏØÏÛÏÌÏÙÏÍÏÎÏÆÏÏÏËğï"},
    {"xiang","ÏëÏñÏàÏòÏóÏãÏìÏä½µÏíÏîÏçÏêÏéÏáÏğÏèÏïÏâÏå"},
    {"xiao","Ğ¡Ğ¦ÏûĞ£Ğ§ÏúÏşĞ¤Ï÷ÏüÏøÏôåĞĞ¢ÏùĞ¥äìÏıóïÏõÏöèÉ"},
    {"xie","Ğ»Ğ©½âĞ´ÑªĞ¬Ğ­Ğ²Ğ°Ò¶ĞªĞ¹ĞµĞ¯Ğ¶Ğ·Ğ±Ğ³ÙôĞ¸Ğ«Ğ¼Ğ®ĞºöÙåâ"},
    {"xin","ĞÄĞÅĞÂĞÁĞÀĞ½Ğ¾Ü°ĞÆ"},
    {"xing","ĞĞĞÇĞÔĞËĞÒĞÑĞÎĞÍĞÌĞÕÊ¡ĞÉĞÈĞÓĞÊ"},
    {"xiong","ĞÖĞ×ĞÛĞÜĞØĞÙĞÚ"},
    {"xiu","ĞİĞŞ³ôĞãĞßËŞĞäĞáĞàĞâĞåßİ"},
    {"xu","ĞèĞíĞëĞøĞòĞéĞêĞ÷ĞóĞîĞïÓõĞğĞìĞöĞæĞñèòĞô"},
    {"xuan","Ñ¡ĞûĞıĞüÈ¯ìÅĞşĞúÑ£äöĞù"},
    {"xue","Ñ§ÑªÑ©Ñ¨Ï÷Ñ¥Ñ¦"},
    {"xun","Ñ°ÑµÑ¶Ñ²Ñ·Ñ¯Ñ¸Ñ­Ñ«Ñ±Ñ¬Ñ³õ¸Ş¹"},
    {"ya","Ñ½ÑÇÑÀÑ¹ÑÅæ«ÑºÑÈÑ¾Ñ»Ñ¼ÑÄÑÂÑÆÑ¿í¼á¬ÑÃÔş"},
    {"yan","ÑÛÑİÑÔÑéÑÏÑáÑÌÑĞÑÚÑÕÑÓÑçÑØÑÒÑÍÑæÑÎÑ×ÑŞÑÊÑàÇ¦ÑÜåûÑÉÑËÑÙëçäÎéÜÒóÑãÑèÑÖ÷Ê÷úÑåØÍ"},
    {"yang","ÑùÑøÑôÑóÑòÑöÑëÑõÑïÑîÑ÷í¦ìÈÑêÑìÑñ"},
    {"yao","ÒªÒ©Ô¿ÑûÒ¡Ò§Ò«Ò£ÑıÑüÒ¥ëÈØ²½ÄçÛÒ¦Ñş"},
    {"ye","Ò²ÒµÒ¹Ò®Ò¯Ò°Ğ°ÒºÒ¶Ò³ÑÊÒ¬Ò­Ò·Ò¸"},
    {"yi","Ò»ÒÔÒâÒÑÒ½ÒÂÒÁÒ×°¬ÒéÒåÒÉÒÅÒÆÒÀÒëÒÕÒìÒäÎ²ÒÌÒÇÒËÒæÒÎÒÛÒÖÒÚÒíÒßÒêÒàß×ÒÄÒÏÒáÒİÒçÒÒÒãòæÒÓÒÈâùÒ¼ÒÙÒïæä"},
    {"yin","ÒòÒôÒıÓ¡ÒøÒşÒõÒûÒöñ«ÒùÒ÷ÒüäÎÒğÒóÒñ"},
    {"ying","Ó¦Ó°Ó¢Ó­Ó®ÓªÓ²Ó¤Ó¥Ó³Ó¬Ó£Ó¯ğĞÓ©İºÓ±Ó«İÓÙø"},
    {"yo","ÓıÓ´à¡"},
    {"yong","ÓÃÓÀÓµÓÂÓ¾Ó¶Ó¿Ó¹ÓÁÓ½"},
    {"you","ÓĞÓÑÓÖÓÉÓÎÓÍÓÒÓÅÓÌÓÈÓÊÓÓÓÄÓÕÓ×ÓÇÓÆßÏöÏÓË÷øØüå¶èÖ"},
    {"yu","ÓÚÓëÓïÓãÓöÓüÔ¤ì¶ÓêÓŞÓıÎ¾ÓîÓäÓàÔ¢ÓûÔ¡ÓòÓèÓşÓñÓúÓôÓùÓéÓğÔ¥Ó÷ÓõÔ£ÓæÓìè¤ÚÍğöìÙÓâÔ¦ÎµÓåÓÙİÇÓóÓßÓçÓØ"},
    {"yuan","Ô±Ô¶Ô­Ô¸ÔºÔªÔ°Ô´Ô®Ô¹Ô²ÔµÔ©Ô¨Ô³æÂãäÔ¬Ô§"},
    {"yue","ËµÀÖÔ¼ÔÂÔ½Ô¿ÔÄÔÃÔ¾ÔÀÔ»"},
    {"yun","Ô±ÔËÔÊÔĞÔÆÔÎÔÉÔÏìÙÔÈÔÌÔÍ"},
    {"za","ÔÓÔÒÔúÕ¦"},
    {"zai","ÔÚÔÙÔØÔÖÔ×áÌÔÔÔÕ"},
    {"zan","ÔÛÔŞÔİÔÜ"},
    {"zang","²ØÔàÔáÔß"},
    {"zao","ÔçÔìÔãÔâÔèÔêÔëÔíÔïÔäÔéÔå"},
    {"ze","ÔğÔòÔñÔóÕ¦ßõ"},
    {"zei","Ôô"},
    {"zen","Ôõ"},
    {"zeng","ÔøÔöÔ÷Ôù"},
    {"zha","²éÕ¨ÔúÕ¦Õ©ÔüÕ£à©Õ¢Õ¥Õ¤Õ§ÔıÔûòÆÔş"},
    {"zhai","ÔñÕ®Õ¬ÕªÕ­Õ¯Õ«"},
    {"zhan","Õ½Õ¾Õ¹Õ²Õ¼Õ´Õ³²üÕ¶Õ¸ÕÀÕµÕ»Õ¿Õ·Õº"},
    {"zhang","³¤ÕÅÕÉÕÆÕÂÕÊÕÏÕËÕÌÕÈÕÇÕÍó¯ÕÃ"},
    {"zhao","×ÅÕÒÕÕÕĞ³¯ÕÙÕÖ×¦Õ×ÕÓÕÔÕÑÕØ"},
    {"zhe","Õâ×ÅÕßÕÛÕÜÕÚÕŞÕá"},
    {"zhen","ÕæÕòÕäÕóÕëÕğÕìÕñÕïÕíÕêëŞÕîÛÚÕåÖ¡"},
    {"zheng","ÕıÖ¤ÕûÕşÕùÖ¢Õ÷ÕüÕõÕöÕôÖ£áçóİ"},
    {"zhi","ÖªÖ»Ö®Ö±Ö¸ÖÁÊ¶ÖÆÖ¹ÖÎÖ§ÖÃÖµÖ°Ö¾ÖÂÖÊÖ½ÖÇÖ¯Ö´Ö·Ö²Ö¥Ö­Ö³Ö©Ö¶Ö¦Ö¬Ö¼Ö«ÖÈÖÉÖºÖ¿ÖÏÖ¨ÖÀáçÖÄÖÍÖËÛ¤ğëÖÅÖÌ"},
    {"zhong","ÖĞÖÖÖØÖÓÖÕÖÚÖÒÖÔÖ×ÖÙ"},
    {"zhou","ÖÜÖİÖŞÖäÖæÖáÖÛÖèÖåÖàÖâÖçÖã"},
    {"zhu","×¡Ö÷Öú×¢ÖøÊõ×£ÊôÖíÖìÖéÖğÖşÖóÖîÖëÜïÖùÖò×¤ÖöÖñÙªÖêÖıÖûä¨Öõ"},
    {"zhua","×¥×¦ÎÎ"},
    {"zhuai","×ª×§"},
    {"zhuan","×ª´«×¨×¬×©×«ãç"},
    {"zhuang","×°×´×²×³×¯×±×®´±"},
    {"zhui","×·×¹×µ×¸×º"},
    {"zhun","×¼"},
    {"zhuo","×Å×À×½×¿×¾ïí×Á×Ä×Æ×Ã"},
    {"zi","×Ó×Ô×Ö×Ê×Ğ×È×Ë×Ì×Ïæ¢Ö¨×É×Õ×Ñ×Ò"},
    {"zong","×Ü×Ù×Ú×İ×Ø×Û"},
    {"zou","×ß×à×á"},
    {"zu","×é×ã×å×è×â×æ×ç×ä"},
    {"zuan","×ê"},
    {"zui","×î×ì×ï×í"},
    {"zun","×ğ×ñ"},
    {"zuo","×ö×÷×ø×ò×ù×ó×ô×Á´é"},
};
static char* Key_input;
static uint16_t Key_input_x;
static uint16_t Key_input_y;
static uint8_t Key_input_size;
static uint8_t Key_input_max_len;
static uint16_t insert_x;
static uint8_t insert_index;
static uint8_t insert_len;
static struct Ui_button* pressed_key[20];
static uint8_t pressed_key_delay[20];
static uint8_t pressed_top;
static uint8_t flag_is_type_pinyin;
static char pinyin_buf[10];
static uint8_t pinyin_len;
static uint8_t pinyin_ptr_number;
static uint16_t this_pinyin_ptr_index;
static struct Ui_button button_26_keys[26];
static struct Ui_button button_caps;
static struct Ui_button button_backspace;
static struct Ui_button button_type;
static struct Ui_button button_lang;
static struct Ui_button button_go_left;
static struct Ui_button button_space;
static struct Ui_button button_go_right;
static struct Ui_button button_dot;
static struct Ui_button button_ok;
static uint8_t keyboard_caps_is_always;
static uint8_t keyboard_case_caps;
static uint8_t keyboard_case_type;
static uint8_t keyboard_case_lang;
static char out_temp[10];
static const char* key_names[8][26]={
    {"q","w","e","r","t","y","u","i","o","p",
     "a","s","d","f","g","h","j","k","l",
     "z","x","c","v","b","n","m"},
    {"q","w","e","r","t","y","u","i","o","p",
     "a","s","d","f","g","h","j","k","l",
     "z","x","c","v","b","n","m"},
    {"1","2","3","4","5","6","7","8","9","0",
     "-","/",";",":","(",")","$","&","@",
     ".","_",",","?","!","'","\""},
    {"1","2","3","4","5","6","7","8","9","0",
     "£¬","¡£","¡¢","£»","£º","£¿","£¡","£¤","£¨",
     "£©","¡¶","¡·","¡°","¡±","¡ª","¡­"},
    {"Q","W","E","R","T","Y","U","I","O","P",
     "A","S","D","F","G","H","J","K","L",
     "Z","X","C","V","B","N","M"},
    {"Q","W","E","R","T","Y","U","I","O","P",
     "A","S","D","F","G","H","J","K","L",
     "Z","X","C","V","B","N","M"},
    {"!","@","#","$","%","^","&","*","(",")",
     "_","+","[","]","{","}","|","\\","~",
     "<",">","/","=","?",";",":"},
    {"£¡","£À","££","£¤","£¥","¡­","£¦","£ª","£¨","£©",
     "£­","£«","¡¾","¡¿","£û","£ı","£ü","¡¢","¡«",
     "¡¶","¡·","£¯","£½","£¿","£»","£º"}
};
static const uint8_t shift_white_data[72]={0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x00,0x00,0x3C,0x00,0x00,0x7E,0x00,0x00,0xE7,0x00,0x80,
0xC3,0x01,0xC0,0x81,0x03,0xE0,0x00,0x07,0x70,0x00,0x0E,0x38,0x00,0x1C,
0x1C,0x00,0x38,0x0E,0x00,0x70,0xFE,0x00,0x7F,0xFC,0x81,0x3F,0xC0,0x81,
0x03,0x80,0x81,0x01,0x80,0x81,0x01,0x80,0x81,0x01,0x80,0xC3,0x01,0x80,
0xFF,0x01,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t shift_black_data[72]={0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x00,0x00,0x3C,0x00,0x00,0x7E,0x00,0x00,0xFF,0x00,0x80,
0xFF,0x01,0xC0,0xFF,0x03,0xE0,0xFF,0x07,0xF0,0xFF,0x0F,0xF8,0xFF,0x1F,
0xFC,0xFF,0x3F,0xFE,0xFF,0x7F,0xFE,0xFF,0x7F,0xFC,0xFF,0x3F,0xC0,0xFF,
0x03,0x80,0xFF,0x01,0x80,0xFF,0x01,0x80,0xFF,0x01,0x80,0xFF,0x01,0x80,
0xFF,0x01,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t back_space_data[72]={0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x80,0xFF,0x3F,0xC0,0xFF,0x7F,0xE0,0x00,0xE0,0x70,
0x00,0xC0,0x38,0x00,0xC0,0x1C,0x0C,0xC3,0x0E,0x9C,0xC3,0x07,0xF8,0xC1,
0x03,0xF0,0xC0,0x03,0xF0,0xC0,0x07,0xF8,0xC1,0x0E,0x9C,0xC3,0x1C,0x0C,
0xC3,0x38,0x00,0xC0,0x70,0x00,0xC0,0xE0,0x00,0xE0,0xC0,0xFF,0x7F,0x80,
0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static void draw_special(uint16_t x,uint16_t y,const uint8_t* special,uint16_t color){
    Lcd_set_window(x,y,24,24);
    Lcd_set_cursor(x,y);
    Lcd_write_start();
    uint8_t temp;const uint8_t* trans=special;
    for(uint8_t i=0;i<72;i++){
        temp=trans[i];
        for(uint8_t j=0;j<8;j++){
            if(temp&0x01){
                Lcd_write(color);
            }
            else{
                Lcd_write(0xD6DB);
            }
            temp>>=1;
        }
    }
    Lcd_set_window(0,0,480,800);
}
static void draw_button_lang(void){
    if(keyboard_case_lang){
        Lcd_draw_chr_chinese(76,755,0xD6D0,24,0x0000,1,0xD6DB);
        Lcd_draw_chr(100,763,'/',16,0x7BEF,1,0xD6DB);
        Lcd_draw_chr_chinese(108,763,0xD3A2,16,0x7BEF,1,0xD6DB);
    }
    else{
        Lcd_draw_chr_chinese(76,755,0xD3A2,24,0x0000,1,0xD6DB);
        Lcd_draw_chr(100,763,'/',16,0x7BEF,1,0xD6DB);
        Lcd_draw_chr_chinese(108,763,0xD6D0,16,0x7BEF,1,0xD6DB);
    }
}
static void draw_button_space(void){
    Lcd_draw_rect(213,770,3,5,0x0000,1);
    Lcd_draw_rect(263,770,3,5,0x0000,1);
    Lcd_draw_rect(213,775,53,3,0x0000,1);
}
static void bg_pic_draw(void){
    for(uint32_t j=0;j<20;j++){Lcd_draw_line(0,520+j,480,0,color_blend(0xFFFF,0x5D1D,j,20));}
    Lcd_draw_rect(0,540,480,240,0x5D1D,1);
    for(uint32_t j=0;j<20;j++){Lcd_draw_line(0,780+j,480,0,color_blend(0x5D1D,0xFFFF,j,20));}
    Lcd_draw_rect(0,520,480,280,0x4A49,0);
    Lcd_draw_rect(1,521,478,278,0x4A49,0);
}
static void draw_button_go_left(void){
    Lcd_draw_chr(134+20-12+6-1,738+27-12,'<',24,0x0000,1,0x0000);
    Lcd_draw_chr(134+20-11+6-1,738+27-12,'<',24,0x0000,1,0x0000);
    Lcd_draw_chr(134+20-13+6-1,738+27-12,'<',24,0x0000,1,0x0000);
}
static void draw_button_go_right(void){
    Lcd_draw_chr(304+20-12+6+1,738+27-12,'>',24,0x0000,1,0x0000);
    Lcd_draw_chr(304+20-11+6+1,738+27-12,'>',24,0x0000,1,0x0000);
    Lcd_draw_chr(304+20-13+6+1,738+27-12,'>',24,0x0000,1,0x0000);
}
static void button_init(void){
    const char* const* key_names_this=key_names[keyboard_case_caps<<2|keyboard_case_type<<1|keyboard_case_lang];
    for(uint8_t i=0;i<26;i++){
        button_26_keys[i].border_width=3;
        button_26_keys[i].text=(char*)key_names_this[i];
        button_26_keys[i].text_size=24;
        button_26_keys[i].color_light=0xFFFF;
        button_26_keys[i].color_dark=0xC639;
        button_26_keys[i].color_ground=0xD6DB;
        button_26_keys[i].color_text=0x0000;
        button_26_keys[i].width=40;
        button_26_keys[i].height=54;
        button_26_keys[i].is_pressed=0;
    }
    for(uint8_t i=0;i<10;i++){
        button_26_keys[i].x=9+i*47;
        button_26_keys[i].y=532;
    }
    for(uint8_t i=10;i<19;i++){
        button_26_keys[i].x=31+(i-10)*47;
        button_26_keys[i].y=601;
    }
    for(uint8_t i=19;i<26;i++){
        button_26_keys[i].x=78+(i-19)*47;
        button_26_keys[i].y=670;
    }
    button_caps.border_width=3;
    button_caps.x=9;
    button_caps.y=670;
    button_caps.height=54;
    button_caps.width=62;
    button_caps.color_light=0xFFFF;
    button_caps.color_dark=0xC639;
    button_caps.color_ground=0xD6DB;
    button_caps.is_pressed=0;
    button_backspace.border_width=3;
    button_backspace.x=407;
    button_backspace.y=670;
    button_backspace.height=54;
    button_backspace.width=64;
    button_backspace.color_light=0xFFFF;
    button_backspace.color_dark=0xC639;
    button_backspace.color_ground=0xD6DB;
    button_backspace.is_pressed=0;
    button_type.border_width=3;
    button_type.x=9;
    button_type.y=738;
    button_type.height=54;
    button_type.width=56;
    button_type.color_light=0xFFFF;
    button_type.color_dark=0xC639;
    button_type.color_ground=0xD6DB;
    if(keyboard_case_type){button_type.text=(char*)"·µ»Ø";}
    else{button_type.text=(char*)"·ûºÅ";}
    button_type.text_size=24;
    button_type.color_text=0x0000;
    button_type.is_pressed=0;
    button_lang.border_width=3;
    button_lang.x=72;
    button_lang.y=738;
    button_lang.height=54;
    button_lang.width=56;
    button_lang.color_light=0xFFFF;
    button_lang.color_dark=0xC639;
    button_lang.color_ground=0xD6DB;
    button_lang.is_pressed=0;
    button_go_left.border_width=3;
    button_go_left.x=134;
    button_go_left.y=738;
    button_go_left.height=54;
    button_go_left.width=40;
    button_go_left.color_light=0xFFFF;
    button_go_left.color_dark=0xC639;
    button_go_left.color_ground=0xD6DB;
    button_go_left.is_pressed=0;
    button_space.border_width=3;
    button_space.x=181;
    button_space.y=738;
    button_space.height=54;
    button_space.width=116;
    button_space.color_light=0xFFFF;
    button_space.color_dark=0xC639;
    button_space.color_ground=0xD6DB;
    button_space.is_pressed=0;
    button_go_right.border_width=3;
    button_go_right.x=304;
    button_go_right.y=738;
    button_go_right.height=54;
    button_go_right.width=40;
    button_go_right.color_light=0xFFFF;
    button_go_right.color_dark=0xC639;
    button_go_right.color_ground=0xD6DB;
    button_go_right.is_pressed=0;
    button_dot.border_width=3;
    button_dot.x=351;
    button_dot.y=738;
    button_dot.height=54;
    button_dot.width=40;
    button_dot.color_light=0xFFFF;
    button_dot.color_dark=0xC639;
    button_dot.color_ground=0xD6DB;
    button_dot.text=(char*)",";
    button_dot.text_size=24;
    button_dot.color_text=0x0000;
    button_dot.is_pressed=0;
    button_ok.border_width=3;
    button_ok.x=397;
    button_ok.y=738;
    button_ok.height=54;
    button_ok.width=74;
    button_ok.color_light=0xFFFF;
    button_ok.color_dark=0xC639;
    button_ok.color_ground=0xD6DB;
    button_ok.text=(char*)"Íê³É";
    button_ok.text_size=24;
    button_ok.color_text=0x0000;
    button_ok.is_pressed=0;
}
static void button_draw(void){
    for(uint8_t i=0;i<26;i++){
        Ui_draw_button_normal(&button_26_keys[i]);
        Ui_draw_button_text(&button_26_keys[i]);
    }
    Ui_draw_button_normal(&button_caps);
    Ui_draw_button_bg(&button_caps);
    draw_special(28,685,shift_white_data,0x0000);
    Ui_draw_button_normal(&button_backspace);
    Ui_draw_button_bg(&button_backspace);
    draw_special(426,685,back_space_data,0x0000);
    Ui_draw_button_normal(&button_type);
    Ui_draw_button_text(&button_type);
    Ui_draw_button_normal(&button_lang);
    Ui_draw_button_bg(&button_lang);
    draw_button_lang();
    Ui_draw_button_normal(&button_go_left);
    Ui_draw_button_bg(&button_go_left);
    draw_button_go_left();
    Ui_draw_button_normal(&button_space);
    Ui_draw_button_bg(&button_space);
    draw_button_space();
    Ui_draw_button_normal(&button_go_right);
    Ui_draw_button_bg(&button_go_right);
    draw_button_go_right();
    Ui_draw_button_normal(&button_dot);
    Ui_draw_button_text(&button_dot);
    Ui_draw_button_normal(&button_ok);
    Ui_draw_button_text(&button_ok);
}
static void button_text_update(void){
    const char* const* key_names_this=key_names[keyboard_case_caps<<2|keyboard_case_type<<1|keyboard_case_lang];
    for(uint8_t i=0;i<26;i++){
        button_26_keys[i].text=(char*)key_names_this[i];
        Ui_draw_button_text(&button_26_keys[i]);
    }
}
static void key_draw_content(void){
    Lcd_draw_str_chinese(Key_input_x,Key_input_y,Key_input,Key_input_size,0x0000,0,0xFFFF);
}
static void key_add_char_no_draw(char add_char){
    for(uint8_t i=insert_len+1;i>insert_index;i--){
        Key_input[i]=Key_input[i-1];
    }
    Key_input[insert_index]=add_char;
    insert_index+=1;insert_len+=1;
    insert_x+=Key_input_size/2;
}
static void key_add_char(char add_char){
    for(uint8_t i=insert_len+1;i>insert_index;i--){
        Key_input[i]=Key_input[i-1];
    }
    Key_input[insert_index]=add_char;
    insert_index+=1;insert_len+=1;
    insert_x+=Key_input_size/2;
    key_draw_content();
}
static void pinyin_clear_draw(void){
    Lcd_draw_rect(0,480,480,40,0xB79B,1);
    Lcd_draw_rect(0,480,480,40,0x4A49,0);
    Lcd_draw_rect(1,481,478,38,0x4A49,0);
}
static void pinyin_update_draw(void){
    Lcd_draw_rect(0,480,480,40,0xB79B,1);
    Lcd_draw_rect(0,480,480,40,0x4A49,0);
    Lcd_draw_rect(1,481,478,38,0x4A49,0);
    for(uint8_t i=0;i<11;i++){
        Lcd_draw_line(i*39+87,480,0,40,0x0000);
        Lcd_draw_line(i*39+88,480,0,40,0x0000);
        Lcd_draw_line(i*39+89,480,0,40,0x0000);
    }
    Lcd_draw_str(8,488,"      ",24,0x0000,0,0xB79B);
    Lcd_draw_str(8,488,pinyin_buf,24,0x0000,0,0xB79B);
    if(this_pinyin_ptr_index==500){return;}
    if(pinyin_ptr_number!=0){
        Lcd_draw_chr(99,488,'<',24,0x0000,1,0xFFFF);
        Lcd_draw_chr(100,488,'<',24,0x0000,1,0xFFFF);
        Lcd_draw_chr(101,488,'<',24,0x0000,1,0xFFFF);
    }
    else{
        Lcd_draw_chr(99,488,'<',24,0xC639,1,0xFFFF);
        Lcd_draw_chr(100,488,'<',24,0xC639,1,0xFFFF);
        Lcd_draw_chr(101,488,'<',24,0xC639,1,0xFFFF);
    }
    const char* x=py_table[this_pinyin_ptr_index].chars;
    uint16_t m=0,k=strlen(x)/2;
    if(pinyin_ptr_number*8+8<k){
        for(uint8_t i=0;i<8;i++){
            m=x[2*(pinyin_ptr_number*8+i)]<<8;
            m|=x[2*(pinyin_ptr_number*8+i)+1];
            Lcd_draw_chr_chinese(i*39+135,488,m,24,0x0000,1,0xFFFF);
        }
        Lcd_draw_chr(452,488,'>',24,0x0000,1,0xFFFF);
        Lcd_draw_chr(453,488,'>',24,0x0000,1,0xFFFF);
        Lcd_draw_chr(454,488,'>',24,0x0000,1,0xFFFF);
    }
    else{
        k-=pinyin_ptr_number*8;
        for(uint8_t i=0;i<k;i++){
            m=x[2*(pinyin_ptr_number*8+i)]<<8;
            m|=x[2*(pinyin_ptr_number*8+i)+1];
            Lcd_draw_chr_chinese(i*39+135,488,m,24,0x0000,1,0xFFFF);
        }
        Lcd_draw_chr(452,488,'>',24,0xC639,1,0xFFFF);
        Lcd_draw_chr(453,488,'>',24,0xC639,1,0xFFFF);
        Lcd_draw_chr(454,488,'>',24,0xC639,1,0xFFFF);
    }
}
static void find_pinyin(void){
    this_pinyin_ptr_index=500;
    for(uint16_t i=0;i<407;i++){
        if(strcmp(py_table[i].py,pinyin_buf)==0){
            this_pinyin_ptr_index=i;
            return;
        }
    }
}
static void pinyin_add_char(char c){
    if(pinyin_len<6){
        pinyin_buf[pinyin_len++]=c;
        pinyin_buf[pinyin_len]='\0';
    }
    find_pinyin();
    pinyin_ptr_number=0;
    pinyin_update_draw();
}
static void pinyin_backspace(void){
    if(pinyin_len==1){
        pinyin_len-=1;
        pinyin_buf[pinyin_len]='\0';
        pinyin_clear_draw();
        flag_is_type_pinyin=0;
        return;
    }
    if(pinyin_len>1){
        pinyin_len--;
        pinyin_buf[pinyin_len]='\0';
    }
    find_pinyin();
    pinyin_ptr_number=0;
    pinyin_update_draw();
}
static void key_add_qwerty(uint8_t in){
    if(insert_len<Key_input_max_len){
        uint8_t all_flag=keyboard_case_caps<<2|keyboard_case_type<<1|keyboard_case_lang;
        if(all_flag==0){
            key_add_char(key_names[0][in][0]);
        }
        else if(all_flag==1){
            if(flag_is_type_pinyin==0){
                flag_is_type_pinyin=1;
                pinyin_len=0;
            }
            pinyin_add_char(key_names[1][in][0]);
        }
        else if(all_flag==2){
            key_add_char(key_names[2][in][0]);
        }
        else if(all_flag==3){
            if(0<in&&in<10){
                key_add_char(key_names[3][in][0]);
            }
            else{
                key_add_char(key_names[3][in][0]);
                key_add_char(key_names[3][in][1]);
            }
        }
        else if(all_flag==4||all_flag==5){
            key_add_char(key_names[4][in][0]);
            if(!keyboard_caps_is_always){
                keyboard_caps_is_always=0;
                keyboard_case_caps=0;
                draw_special(28,685,shift_white_data,0x0000);
                button_text_update();
            }
        }
        else if(all_flag==6){
            key_add_char(key_names[6][in][0]);
            if(!keyboard_caps_is_always){
                keyboard_caps_is_always=0;
                keyboard_case_caps=0;
                draw_special(28,685,shift_white_data,0x0000);
                button_text_update();
            }
        }
        else if(all_flag==7){
            key_add_char(key_names[7][in][0]);
            key_add_char(key_names[7][in][1]);
            if(!keyboard_caps_is_always){
                keyboard_caps_is_always=0;
                keyboard_case_caps=0;
                draw_special(28,685,shift_white_data,0x0000);
                button_text_update();
            }
        }
    }
}
void Key_board(char* input_str,uint16_t input_x,uint16_t input_y,uint8_t input_size,uint8_t max_len,uint8_t keyboard_case){
    uint32_t time_tick_handle=0;
    uint16_t x,y;
    uint8_t flag_need_press;
    struct Ui_button* pressed_key_cha;
    Key_input=input_str;
    Key_input_x=input_x;
    Key_input_y=input_y;
    Key_input_size=input_size;
    Key_input_max_len=max_len;
    insert_index=strlen(input_str);
    insert_len=insert_index;
    insert_x=Key_input_x+insert_index*Key_input_size/2;
    keyboard_caps_is_always=0;
    keyboard_case_caps=(keyboard_case&0x04)>>2;
    keyboard_case_type=(keyboard_case&0x02)>>1;
    keyboard_case_lang=keyboard_case&0x01;
    button_init();
    bg_pic_draw();
    button_draw();
    pressed_top=0;
    flag_is_type_pinyin=0;
    Lcd_draw_rect(Key_input_x,Key_input_y,Key_input_size*max_len/2,Key_input_size,0xFFFF,1);
    key_draw_content();
    pinyin_clear_draw();
    while(1){
        if(pressed_top==1){
            pressed_key_delay[0]-=1;
            pressed_key[0]->is_pressed-=1;
            if(pressed_key_delay[0]==0){
                Ui_draw_button_normal(pressed_key[0]);
                pressed_top=0;
            }
        }
        else if(pressed_top>1){
            for(uint8_t i=0;i<pressed_top;i++){
                pressed_key_delay[i]-=1;
                pressed_key[0]->is_pressed-=1;
            }
            if(pressed_key_delay[0]==0){
                Ui_draw_button_normal(pressed_key[0]);
                for(uint8_t i=1;i<pressed_top;i++){
                    pressed_key_delay[i-1]=pressed_key_delay[i];
                    pressed_key[i-1]=pressed_key[i];
                }
                pressed_top-=1;
            }
        }
        if(pressed_top==0&&time_tick_handle==50){
            for(uint8_t i=0;i<26;i++){button_26_keys[i].is_pressed=0;}
            button_caps.is_pressed=0;
            button_backspace.is_pressed=0;
            button_type.is_pressed=0;
            button_lang.is_pressed=0;
            button_go_left.is_pressed=0;
            button_space.is_pressed=0;
            button_go_right.is_pressed=0;
            button_dot.is_pressed=0;
            button_ok.is_pressed=0;
        }
        time_tick_handle+=1;
        if(time_tick_handle==30){
            Lcd_draw_line(insert_x,Key_input_y,0,input_size,0x0000);
        }
        else if(time_tick_handle==60){
            time_tick_handle=0;
            Lcd_draw_line(insert_x,Key_input_y,0,input_size,0xFFFF);
        }
        if(Touch_get()){
            flag_need_press=1;
            x=Touch_get_x[0];
            y=Touch_get_y[0];
            pressed_key_cha=NULL;
            if(532<=y&&y<586){
                for(uint8_t i=0;i<10;i++){
                    if(Ui_draw_button_get_axis_x(&button_26_keys[i],x)){
                        pressed_key_cha=&button_26_keys[i];
                        key_add_qwerty(i);
                        break;
                    }
                }
            }
            else if(601<=y&&y<655){
                for(uint8_t i=10;i<19;i++){
                    if(Ui_draw_button_get_axis_x(&button_26_keys[i],x)){
                        pressed_key_cha=&button_26_keys[i];
                        key_add_qwerty(i);
                        break;
                    }
                }
            }
            else if(670<=y&&y<724){
                for(uint8_t i=19;i<26;i++){
                    if(Ui_draw_button_get_axis_x(&button_26_keys[i],x)){
                        pressed_key_cha=&button_26_keys[i];
                        key_add_qwerty(i);
                        break;
                    }
                }
                if(Ui_draw_button_get_axis_x(&button_caps,x)){
                    pressed_key_cha=&button_caps;
                    Ui_draw_button_pressed(pressed_key_cha);
                    flag_need_press=0;
                    if(!flag_is_type_pinyin){
                        if(keyboard_case_caps==0){
                            keyboard_caps_is_always=0;
                            keyboard_case_caps=1;
                            draw_special(28,685,shift_black_data,0x0000);
                        }
                        else{
                            if(keyboard_caps_is_always==0){
                                keyboard_caps_is_always=1;
                                keyboard_case_caps=1;
                                draw_special(28,685,shift_black_data,0x001F);
                            }
                            else{
                                keyboard_caps_is_always=0;
                                keyboard_case_caps=0;
                                draw_special(28,685,shift_white_data,0x0000);
                            }
                        }
                        button_text_update();
                    }
                }
                else if(Ui_draw_button_get_axis_x(&button_backspace,x)){
                    if(!flag_is_type_pinyin){
                        flag_need_press=0;
                        Ui_draw_button_pressed(&button_backspace);
                        if(insert_index>1&&is_chinese_byte(Key_input,insert_index-1)){
                            for(uint8_t i=insert_index;i<=insert_len+1;i++){
                                Key_input[i-2]=Key_input[i];
                            }
                            insert_index-=2;
                            insert_x-=Key_input_size;
                            insert_len-=2;
                            Lcd_draw_rect(Key_input_x+insert_len*Key_input_size/2,Key_input_y,Key_input_size+2,Key_input_size,0xFFFF,1);
                        }
                        else if(insert_index>0){
                            for(uint8_t i=insert_index;i<=insert_len;i++){
                                Key_input[i-1]=Key_input[i];
                            }
                            insert_index-=1;
                            insert_x-=Key_input_size/2;
                            insert_len-=1;
                            Lcd_draw_rect(Key_input_x+insert_len*Key_input_size/2,Key_input_y,Key_input_size/2+2,Key_input_size,0xFFFF,1);
                        }
                        key_draw_content();
                    }
                    else{
                        pinyin_backspace();
                    }
                    pressed_key_cha=&button_backspace;
                }
            }
            else if(738<=y&&y<792){
                if(Ui_draw_button_get_axis_x(&button_type,x)){
                    pressed_key_cha=&button_type;
                    if(!flag_is_type_pinyin){
                        flag_need_press=0;
                        Ui_draw_button_pressed(&button_type);
                        if(keyboard_case_type){
                            button_type.text=(char*)"·ûºÅ";
                        }
                        else{
                            button_type.text=(char*)"·µ»Ø";
                        }
                        keyboard_case_type=1-keyboard_case_type;
                        Ui_draw_button_text(&button_type);
                        button_text_update();
                    }
                }
                else if(Ui_draw_button_get_axis_x(&button_lang,x)){
                    pressed_key_cha=&button_lang;
                    if(!flag_is_type_pinyin){
                        keyboard_case_lang=1-keyboard_case_lang;
                        Ui_draw_button_bg(&button_lang);
                        draw_button_lang();
                        button_text_update();
                    }
                    flag_need_press=0;
                    Ui_draw_button_pressed(&button_lang);
                }
                else if(Ui_draw_button_get_axis_x(&button_go_left,x)){
                    flag_need_press=0;
                    Ui_draw_button_pressed(&button_go_left);
                    if(insert_index>1&&is_chinese_byte(Key_input,insert_index-1)){
                        Lcd_draw_line(insert_x,Key_input_y,0,input_size,0xFFFF);
                        out_temp[0]=Key_input[insert_index];
                        out_temp[1]=Key_input[insert_index+1];
                        out_temp[2]='\0';
                        Lcd_draw_str(insert_x,Key_input_y,out_temp,input_size,0x0000,0,0xFFFF);
                        insert_index-=2;
                        insert_x-=Key_input_size;
                    }
                    else if(insert_index>0){
                            Lcd_draw_line(insert_x,Key_input_y,0,input_size,0xFFFF);
                            out_temp[0]=Key_input[insert_index];
                            out_temp[1]=Key_input[insert_index+1];
                            out_temp[2]='\0';
                            Lcd_draw_str(insert_x,Key_input_y,out_temp,input_size,0x0000,0,0xFFFF);
                            insert_index-=1;
                            insert_x-=Key_input_size/2;
                        }
                    pressed_key_cha=&button_go_left;
                }
                else if(Ui_draw_button_get_axis_x(&button_space,x)){
                    if(!flag_is_type_pinyin){
                        flag_need_press=0;
                        Ui_draw_button_pressed(&button_space);
                        key_add_char(' ');
                        pressed_key_cha=&button_space;
                    }
                }
                else if(Ui_draw_button_get_axis_x(&button_go_right,x)){
                    flag_need_press=0;
                    Ui_draw_button_pressed(&button_go_right);
                    if(is_chinese_byte(Key_input,insert_index)&&insert_index<insert_len-1){
                        Lcd_draw_line(insert_x,Key_input_y,0,input_size,0xFFFF);
                        out_temp[0]=Key_input[insert_index];
                        out_temp[1]=Key_input[insert_index+1];
                        out_temp[2]='\0';
                        Lcd_draw_str(insert_x,Key_input_y,out_temp,input_size,0x0000,0,0xFFFF);
                        insert_index+=2;
                        insert_x+=Key_input_size;
                    }
                    else if(insert_index<insert_len){
                        Lcd_draw_line(insert_x,Key_input_y,0,input_size,0xFFFF);
                        out_temp[0]=Key_input[insert_index];
                        out_temp[1]=Key_input[insert_index+1];
                        out_temp[2]='\0';
                        Lcd_draw_str(insert_x,Key_input_y,out_temp,input_size,0x0000,0,0xFFFF);
                        insert_index+=1;
                        insert_x+=Key_input_size/2;
                    }
                    pressed_key_cha=&button_go_right;
                }
                else if(Ui_draw_button_get_axis_x(&button_dot,x)){
                    if(!flag_is_type_pinyin){
                        flag_need_press=0;
                        Ui_draw_button_pressed(&button_dot);
                        if(keyboard_case_lang){
                            key_add_char(0xA3);
                            key_add_char(0xAC);
                        }
                        else{
                            key_add_char(',');
                        }
                    }
                    pressed_key_cha=&button_dot;
                }
                else if(Ui_draw_button_get_axis_x(&button_ok,x)){
                    if(!flag_is_type_pinyin){
                        Ui_draw_button_pressed(&button_ok);
                        delay_ms(80);
                        Ui_draw_button_normal(&button_ok);
                        delay_ms(80);
                        Lcd_draw_rect(input_x,input_y,max_len*input_size/2,input_size,0xFFFF,1);
                        Lcd_draw_str_chinese(Key_input_x,Key_input_y,Key_input,Key_input_size,0x0000,0,0xFFFF);
                        Lcd_draw_rect(0,480,480,320,0xFFFF,1);
                        return;
                    }
                }
            }
            else if(flag_is_type_pinyin&&482<=y&&y<=518){
                if(this_pinyin_ptr_index!=500){
                    if(88<=x&&x<127){
                        if(pinyin_ptr_number>0){
                            pinyin_ptr_number-=1;
                            pinyin_update_draw();
                            delay_ms(140);
                        }
                    }
                    else if(439<=x&&x<478){
                        if(pinyin_ptr_number*8+8<strlen(py_table[this_pinyin_ptr_index].chars)/2){
                            pinyin_ptr_number+=1;
                            pinyin_update_draw();
                            delay_ms(140);
                        }
                    }
                    else if(127<=x&&x<439){
                        uint16_t inde_key=(x-88)/39-1;
                        const char* x=py_table[this_pinyin_ptr_index].chars;
                        if(pinyin_ptr_number*8+inde_key<strlen(x)/2){
                            key_add_char_no_draw(x[2*(pinyin_ptr_number*8+inde_key)]);
                            key_add_char(x[2*(pinyin_ptr_number*8+inde_key)+1]);
                            pinyin_clear_draw();
                            flag_is_type_pinyin=0;
                        }
                    }
                }
            }
            if(pressed_key_cha!=NULL){
                time_tick_handle=29;
                pressed_key_cha->is_pressed+=8;
                pressed_key[pressed_top]=pressed_key_cha;
                pressed_key_delay[pressed_top]=8;
                pressed_top+=1;
                if(flag_need_press){
                    Ui_draw_button_pressed(pressed_key_cha);
                }
                delay_ms(10);
            }
        }
        delay_ms(10);
    }
}
