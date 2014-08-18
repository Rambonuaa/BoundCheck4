/* +++Date last modified: 05-Jul-1997 */

/*
**        A Pratt-Boyer-Moore string search, written by Jerry Coffin
**  sometime or other in 1991.  Removed from original program, and
**  (incorrectly) rewritten for separate, generic use in early 1992.
**  Corrected with help from Thad Smith, late March and early
**  April 1992...hopefully it's correct this time. Revised by Bob Stout.
**
**  This is hereby placed in the Public Domain by its author.
**
**  10/21/93 rdg  Fixed bug found by Jeff Dunlop
*/

#include <stddef.h>
#include <string.h>
#include <limits.h>

static size_t table[UCHAR_MAX + 1];
static size_t len;
static char *findme;

/*
**  Call this with the string to locate to initialize the table
*/

void init_search(const char *string)
{
      size_t i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


      len = strlen(string);
      for (i = 0; i <= UCHAR_MAX; i++)                      /* rdg 10/93 */
            table[i] = len;
      for (i = 0; i < len; i++)
            table[(unsigned char)string[i]] = len - i - 1;
      findme = (char *)string;
__boundcheck_metadata_trans_check((void *)(findme),(void *)(string),(void *)((char *)string));

}

/*
**  Call this with a buffer to search
*/

char *strsearch(const char *string)
{
      register size_t shift;


      register size_t pos = len - 1;


      char *here;
__boundcheck_metadata_store((void *)(&here),(void *)((size_t)(&here)+sizeof(here)*8-1));

      size_t limit=strlen(string);
__boundcheck_metadata_store((void *)(&limit),(void *)((size_t)(&limit)+sizeof(limit)*8-1));


      while (pos < limit)
      {
            while( pos < limit &&
                  (shift = table[(unsigned char)string[pos]]) > 0)
            {
                  pos += shift;
            }
            if (0 == shift)
            {
                  if (0 == strncmp(findme,
                        here = (char *)&string[__boundcheck_ptr_cast_to_array_reference(60,48,"strsearch",(void *)(string),(void *)(string+pos-len+1),pos-len+1)], len))
                  {
                        return(here);
                  }
                  else  pos++;
            }
      }
      return NULL;
}

#include <stdio.h>

void __global_variables_init(){
__boundcheck_metadata_store((void *)(&table),(void *)((size_t)(&table)+sizeof(table)*8-1));
__boundcheck_metadata_store((void *)(&len),(void *)((size_t)(&len)+sizeof(len)*8-1));
__boundcheck_metadata_store((void *)(&findme),(void *)((size_t)(&findme)+sizeof(findme)*8-1));
}
main()
{
	__global_variables_init();

      char *here;
__boundcheck_metadata_store((void *)(&here),(void *)((size_t)(&here)+sizeof(here)*8-1));

      char *__tmp_string_1331="me";
 __boundcheck_metadata_store((void *)(__tmp_string_1331),(void *)(__tmp_string_1331+2));
char *__tmp_string_1330="more";
 __boundcheck_metadata_store((void *)(__tmp_string_1330),(void *)(__tmp_string_1330+4));
char *__tmp_string_1329="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_1329),(void *)(__tmp_string_1329+5));
char *__tmp_string_1328="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1328),(void *)(__tmp_string_1328+3));
char *__tmp_string_1327="past";
 __boundcheck_metadata_store((void *)(__tmp_string_1327),(void *)(__tmp_string_1327+4));
char *__tmp_string_1326="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_1326),(void *)(__tmp_string_1326+10));
char *__tmp_string_1325="with";
 __boundcheck_metadata_store((void *)(__tmp_string_1325),(void *)(__tmp_string_1325+4));
char *__tmp_string_1324="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_1324),(void *)(__tmp_string_1324+7));
char *__tmp_string_1323="time";
 __boundcheck_metadata_store((void *)(__tmp_string_1323),(void *)(__tmp_string_1323+4));
char *__tmp_string_1322="too";
 __boundcheck_metadata_store((void *)(__tmp_string_1322),(void *)(__tmp_string_1322+3));
char *__tmp_string_1321="one";
 __boundcheck_metadata_store((void *)(__tmp_string_1321),(void *)(__tmp_string_1321+3));
char *__tmp_string_1320="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_1320),(void *)(__tmp_string_1320+6));
char *__tmp_string_1319="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_1319),(void *)(__tmp_string_1319+5));
char *__tmp_string_1318="support";
 __boundcheck_metadata_store((void *)(__tmp_string_1318),(void *)(__tmp_string_1318+7));
char *__tmp_string_1317="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1317),(void *)(__tmp_string_1317+4));
char *__tmp_string_1316="their";
 __boundcheck_metadata_store((void *)(__tmp_string_1316),(void *)(__tmp_string_1316+5));
char *__tmp_string_1315="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_1315),(void *)(__tmp_string_1315+10));
char *__tmp_string_1314="size";
 __boundcheck_metadata_store((void *)(__tmp_string_1314),(void *)(__tmp_string_1314+4));
char *__tmp_string_1313="get";
 __boundcheck_metadata_store((void *)(__tmp_string_1313),(void *)(__tmp_string_1313+3));
char *__tmp_string_1312="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_1312),(void *)(__tmp_string_1312+11));
char *__tmp_string_1311="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_1311),(void *)(__tmp_string_1311+6));
char *__tmp_string_1310="before";
 __boundcheck_metadata_store((void *)(__tmp_string_1310),(void *)(__tmp_string_1310+6));
char *__tmp_string_1309="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_1309),(void *)(__tmp_string_1309+8));
char *__tmp_string_1308="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1308),(void *)(__tmp_string_1308+2));
char *__tmp_string_1307="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1307),(void *)(__tmp_string_1307+3));
char *__tmp_string_1306="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1306),(void *)(__tmp_string_1306+3));
char *__tmp_string_1305="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1305),(void *)(__tmp_string_1305+3));
char *__tmp_string_1304="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1304),(void *)(__tmp_string_1304+3));
char *__tmp_string_1303="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1303),(void *)(__tmp_string_1303+3));
char *__tmp_string_1302="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1302),(void *)(__tmp_string_1302+3));
char *__tmp_string_1301="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1301),(void *)(__tmp_string_1301+3));
char *__tmp_string_1300="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1300),(void *)(__tmp_string_1300+3));
char *__tmp_string_1299="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1299),(void *)(__tmp_string_1299+3));
char *__tmp_string_1298="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1298),(void *)(__tmp_string_1298+3));
char *__tmp_string_1297="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1297),(void *)(__tmp_string_1297+3));
char *__tmp_string_1296="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1296),(void *)(__tmp_string_1296+3));
char *__tmp_string_1295="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1295),(void *)(__tmp_string_1295+3));
char *__tmp_string_1294="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1294),(void *)(__tmp_string_1294+3));
char *__tmp_string_1293="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1293),(void *)(__tmp_string_1293+3));
char *__tmp_string_1292="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_1292),(void *)(__tmp_string_1292+4));
char *__tmp_string_1291="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1291),(void *)(__tmp_string_1291+6));
char *__tmp_string_1290="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1290),(void *)(__tmp_string_1290+3));
char *__tmp_string_1289="room";
 __boundcheck_metadata_store((void *)(__tmp_string_1289),(void *)(__tmp_string_1289+4));
char *__tmp_string_1288="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_1288),(void *)(__tmp_string_1288+7));
char *__tmp_string_1287="own";
 __boundcheck_metadata_store((void *)(__tmp_string_1287),(void *)(__tmp_string_1287+3));
char *__tmp_string_1286="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_1286),(void *)(__tmp_string_1286+8));
char *__tmp_string_1285="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1285),(void *)(__tmp_string_1285+3));
char *__tmp_string_1284="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1284),(void *)(__tmp_string_1284+2));
char *__tmp_string_1283="other";
 __boundcheck_metadata_store((void *)(__tmp_string_1283),(void *)(__tmp_string_1283+5));
char *__tmp_string_1282="be";
 __boundcheck_metadata_store((void *)(__tmp_string_1282),(void *)(__tmp_string_1282+2));
char *__tmp_string_1281="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1281),(void *)(__tmp_string_1281+2));
char *__tmp_string_1280="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_1280),(void *)(__tmp_string_1280+5));
char *__tmp_string_1279="are";
 __boundcheck_metadata_store((void *)(__tmp_string_1279),(void *)(__tmp_string_1279+3));
char *__tmp_string_1278="either";
 __boundcheck_metadata_store((void *)(__tmp_string_1278),(void *)(__tmp_string_1278+6));
char *__tmp_string_1277="much";
 __boundcheck_metadata_store((void *)(__tmp_string_1277),(void *)(__tmp_string_1277+4));
char *__tmp_string_1276="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_1276),(void *)(__tmp_string_1276+12));
char *__tmp_string_1275="your";
 __boundcheck_metadata_store((void *)(__tmp_string_1275),(void *)(__tmp_string_1275+4));
char *__tmp_string_1274="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1274),(void *)(__tmp_string_1274+3));
char *__tmp_string_1273="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1273),(void *)(__tmp_string_1273+3));
char *__tmp_string_1272="may";
 __boundcheck_metadata_store((void *)(__tmp_string_1272),(void *)(__tmp_string_1272+3));
char *__tmp_string_1271="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1271),(void *)(__tmp_string_1271+5));
char *__tmp_string_1270="them";
 __boundcheck_metadata_store((void *)(__tmp_string_1270),(void *)(__tmp_string_1270+4));
char *__tmp_string_1269="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1269),(void *)(__tmp_string_1269+3));
char *__tmp_string_1268="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_1268),(void *)(__tmp_string_1268+3));
char *__tmp_string_1267="40";
 __boundcheck_metadata_store((void *)(__tmp_string_1267),(void *)(__tmp_string_1267+2));
char *__tmp_string_1266="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1266),(void *)(__tmp_string_1266+3));
char *__tmp_string_1265="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1265),(void *)(__tmp_string_1265+3));
char *__tmp_string_1264="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_1264),(void *)(__tmp_string_1264+3));
char *__tmp_string_1263="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_1263),(void *)(__tmp_string_1263+3));
char *__tmp_string_1262="life";
 __boundcheck_metadata_store((void *)(__tmp_string_1262),(void *)(__tmp_string_1262+4));
char *__tmp_string_1261="what";
 __boundcheck_metadata_store((void *)(__tmp_string_1261),(void *)(__tmp_string_1261+4));
char *__tmp_string_1260="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_1260),(void *)(__tmp_string_1260+6));
char *__tmp_string_1259="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_1259),(void *)(__tmp_string_1259+2));
char *__tmp_string_1258="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_1258),(void *)(__tmp_string_1258+3));
char *__tmp_string_1257="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_1257),(void *)(__tmp_string_1257+3));
char *__tmp_string_1256="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_1256),(void *)(__tmp_string_1256+7));
char *__tmp_string_1255="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_1255),(void *)(__tmp_string_1255+6));
char *__tmp_string_1254="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1254),(void *)(__tmp_string_1254+8));
char *__tmp_string_1253="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1253),(void *)(__tmp_string_1253+3));
char *__tmp_string_1252="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_1252),(void *)(__tmp_string_1252+2));
char *__tmp_string_1251="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1251),(void *)(__tmp_string_1251+3));
char *__tmp_string_1250="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1250),(void *)(__tmp_string_1250+3));
char *__tmp_string_1249="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_1249),(void *)(__tmp_string_1249+3));
char *__tmp_string_1248="people";
 __boundcheck_metadata_store((void *)(__tmp_string_1248),(void *)(__tmp_string_1248+6));
char *__tmp_string_1247="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_1247),(void *)(__tmp_string_1247+5));
char *__tmp_string_1246="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_1246),(void *)(__tmp_string_1246+2));
char *__tmp_string_1245="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_1245),(void *)(__tmp_string_1245+4));
char *__tmp_string_1244="4";
 __boundcheck_metadata_store((void *)(__tmp_string_1244),(void *)(__tmp_string_1244+1));
char *__tmp_string_1243="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_1243),(void *)(__tmp_string_1243+4));
char *__tmp_string_1242="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_1242),(void *)(__tmp_string_1242+2));
char *__tmp_string_1241="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1241),(void *)(__tmp_string_1241+3));
char *__tmp_string_1240="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_1240),(void *)(__tmp_string_1240+2));
char *__tmp_string_1239="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1239),(void *)(__tmp_string_1239+3));
char *__tmp_string_1238="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1238),(void *)(__tmp_string_1238+3));
char *__tmp_string_1237="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_1237),(void *)(__tmp_string_1237+3));
char *__tmp_string_1236="th";
 __boundcheck_metadata_store((void *)(__tmp_string_1236),(void *)(__tmp_string_1236+2));
char *__tmp_string_1235="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_1235),(void *)(__tmp_string_1235+4));
char *__tmp_string_1234="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_1234),(void *)(__tmp_string_1234+3));
char *__tmp_string_1233="an";
 __boundcheck_metadata_store((void *)(__tmp_string_1233),(void *)(__tmp_string_1233+2));
char *__tmp_string_1232="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_1232),(void *)(__tmp_string_1232+9));
char *__tmp_string_1231="way";
 __boundcheck_metadata_store((void *)(__tmp_string_1231),(void *)(__tmp_string_1231+3));
char *__tmp_string_1230="an";
 __boundcheck_metadata_store((void *)(__tmp_string_1230),(void *)(__tmp_string_1230+2));
char *__tmp_string_1229="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_1229),(void *)(__tmp_string_1229+3));
char *__tmp_string_1228="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1228),(void *)(__tmp_string_1228+3));
char *__tmp_string_1227="20";
 __boundcheck_metadata_store((void *)(__tmp_string_1227),(void *)(__tmp_string_1227+2));
char *__tmp_string_1226="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_1226),(void *)(__tmp_string_1226+5));
char *__tmp_string_1225="and";
 __boundcheck_metadata_store((void *)(__tmp_string_1225),(void *)(__tmp_string_1225+3));
char *__tmp_string_1224="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_1224),(void *)(__tmp_string_1224+2));
char *__tmp_string_1223="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1223),(void *)(__tmp_string_1223+5));
char *__tmp_string_1222="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_1222),(void *)(__tmp_string_1222+3));
char *__tmp_string_1221="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_1221),(void *)(__tmp_string_1221+5));
char *__tmp_string_1220="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1220),(void *)(__tmp_string_1220+6));
char *__tmp_string_1219="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1219),(void *)(__tmp_string_1219+6));
char *__tmp_string_1218="where";
 __boundcheck_metadata_store((void *)(__tmp_string_1218),(void *)(__tmp_string_1218+5));
char *__tmp_string_1217="have";
 __boundcheck_metadata_store((void *)(__tmp_string_1217),(void *)(__tmp_string_1217+4));
char *__tmp_string_1216="long";
 __boundcheck_metadata_store((void *)(__tmp_string_1216),(void *)(__tmp_string_1216+4));
char *__tmp_string_1215="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1215),(void *)(__tmp_string_1215+3));
char *__tmp_string_1214="for";
 __boundcheck_metadata_store((void *)(__tmp_string_1214),(void *)(__tmp_string_1214+3));
char *__tmp_string_1213="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_1213),(void *)(__tmp_string_1213+4));
char *__tmp_string_1212="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_1212),(void *)(__tmp_string_1212+4));
char *__tmp_string_1211="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_1211),(void *)(__tmp_string_1211+4));
char *__tmp_string_1210="me";
 __boundcheck_metadata_store((void *)(__tmp_string_1210),(void *)(__tmp_string_1210+2));
char *__tmp_string_1209="more";
 __boundcheck_metadata_store((void *)(__tmp_string_1209),(void *)(__tmp_string_1209+4));
char *__tmp_string_1208="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_1208),(void *)(__tmp_string_1208+5));
char *__tmp_string_1207="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1207),(void *)(__tmp_string_1207+3));
char *__tmp_string_1206="past";
 __boundcheck_metadata_store((void *)(__tmp_string_1206),(void *)(__tmp_string_1206+4));
char *__tmp_string_1205="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_1205),(void *)(__tmp_string_1205+10));
char *__tmp_string_1204="with";
 __boundcheck_metadata_store((void *)(__tmp_string_1204),(void *)(__tmp_string_1204+4));
char *__tmp_string_1203="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_1203),(void *)(__tmp_string_1203+7));
char *__tmp_string_1202="time";
 __boundcheck_metadata_store((void *)(__tmp_string_1202),(void *)(__tmp_string_1202+4));
char *__tmp_string_1201="too";
 __boundcheck_metadata_store((void *)(__tmp_string_1201),(void *)(__tmp_string_1201+3));
char *__tmp_string_1200="one";
 __boundcheck_metadata_store((void *)(__tmp_string_1200),(void *)(__tmp_string_1200+3));
char *__tmp_string_1199="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_1199),(void *)(__tmp_string_1199+6));
char *__tmp_string_1198="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_1198),(void *)(__tmp_string_1198+5));
char *__tmp_string_1197="support";
 __boundcheck_metadata_store((void *)(__tmp_string_1197),(void *)(__tmp_string_1197+7));
char *__tmp_string_1196="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1196),(void *)(__tmp_string_1196+4));
char *__tmp_string_1195="their";
 __boundcheck_metadata_store((void *)(__tmp_string_1195),(void *)(__tmp_string_1195+5));
char *__tmp_string_1194="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_1194),(void *)(__tmp_string_1194+10));
char *__tmp_string_1193="size";
 __boundcheck_metadata_store((void *)(__tmp_string_1193),(void *)(__tmp_string_1193+4));
char *__tmp_string_1192="get";
 __boundcheck_metadata_store((void *)(__tmp_string_1192),(void *)(__tmp_string_1192+3));
char *__tmp_string_1191="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_1191),(void *)(__tmp_string_1191+11));
char *__tmp_string_1190="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_1190),(void *)(__tmp_string_1190+6));
char *__tmp_string_1189="before";
 __boundcheck_metadata_store((void *)(__tmp_string_1189),(void *)(__tmp_string_1189+6));
char *__tmp_string_1188="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_1188),(void *)(__tmp_string_1188+8));
char *__tmp_string_1187="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1187),(void *)(__tmp_string_1187+2));
char *__tmp_string_1186="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1186),(void *)(__tmp_string_1186+3));
char *__tmp_string_1185="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1185),(void *)(__tmp_string_1185+3));
char *__tmp_string_1184="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1184),(void *)(__tmp_string_1184+3));
char *__tmp_string_1183="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1183),(void *)(__tmp_string_1183+3));
char *__tmp_string_1182="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1182),(void *)(__tmp_string_1182+3));
char *__tmp_string_1181="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1181),(void *)(__tmp_string_1181+3));
char *__tmp_string_1180="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1180),(void *)(__tmp_string_1180+3));
char *__tmp_string_1179="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1179),(void *)(__tmp_string_1179+3));
char *__tmp_string_1178="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1178),(void *)(__tmp_string_1178+3));
char *__tmp_string_1177="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1177),(void *)(__tmp_string_1177+3));
char *__tmp_string_1176="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1176),(void *)(__tmp_string_1176+3));
char *__tmp_string_1175="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1175),(void *)(__tmp_string_1175+3));
char *__tmp_string_1174="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1174),(void *)(__tmp_string_1174+3));
char *__tmp_string_1173="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1173),(void *)(__tmp_string_1173+3));
char *__tmp_string_1172="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1172),(void *)(__tmp_string_1172+3));
char *__tmp_string_1171="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_1171),(void *)(__tmp_string_1171+4));
char *__tmp_string_1170="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1170),(void *)(__tmp_string_1170+6));
char *__tmp_string_1169="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1169),(void *)(__tmp_string_1169+3));
char *__tmp_string_1168="room";
 __boundcheck_metadata_store((void *)(__tmp_string_1168),(void *)(__tmp_string_1168+4));
char *__tmp_string_1167="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_1167),(void *)(__tmp_string_1167+7));
char *__tmp_string_1166="own";
 __boundcheck_metadata_store((void *)(__tmp_string_1166),(void *)(__tmp_string_1166+3));
char *__tmp_string_1165="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_1165),(void *)(__tmp_string_1165+8));
char *__tmp_string_1164="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1164),(void *)(__tmp_string_1164+3));
char *__tmp_string_1163="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1163),(void *)(__tmp_string_1163+2));
char *__tmp_string_1162="other";
 __boundcheck_metadata_store((void *)(__tmp_string_1162),(void *)(__tmp_string_1162+5));
char *__tmp_string_1161="be";
 __boundcheck_metadata_store((void *)(__tmp_string_1161),(void *)(__tmp_string_1161+2));
char *__tmp_string_1160="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1160),(void *)(__tmp_string_1160+2));
char *__tmp_string_1159="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_1159),(void *)(__tmp_string_1159+5));
char *__tmp_string_1158="are";
 __boundcheck_metadata_store((void *)(__tmp_string_1158),(void *)(__tmp_string_1158+3));
char *__tmp_string_1157="either";
 __boundcheck_metadata_store((void *)(__tmp_string_1157),(void *)(__tmp_string_1157+6));
char *__tmp_string_1156="much";
 __boundcheck_metadata_store((void *)(__tmp_string_1156),(void *)(__tmp_string_1156+4));
char *__tmp_string_1155="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_1155),(void *)(__tmp_string_1155+12));
char *__tmp_string_1154="your";
 __boundcheck_metadata_store((void *)(__tmp_string_1154),(void *)(__tmp_string_1154+4));
char *__tmp_string_1153="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1153),(void *)(__tmp_string_1153+3));
char *__tmp_string_1152="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1152),(void *)(__tmp_string_1152+3));
char *__tmp_string_1151="may";
 __boundcheck_metadata_store((void *)(__tmp_string_1151),(void *)(__tmp_string_1151+3));
char *__tmp_string_1150="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1150),(void *)(__tmp_string_1150+5));
char *__tmp_string_1149="them";
 __boundcheck_metadata_store((void *)(__tmp_string_1149),(void *)(__tmp_string_1149+4));
char *__tmp_string_1148="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1148),(void *)(__tmp_string_1148+3));
char *__tmp_string_1147="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_1147),(void *)(__tmp_string_1147+3));
char *__tmp_string_1146="40";
 __boundcheck_metadata_store((void *)(__tmp_string_1146),(void *)(__tmp_string_1146+2));
char *__tmp_string_1145="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1145),(void *)(__tmp_string_1145+3));
char *__tmp_string_1144="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1144),(void *)(__tmp_string_1144+3));
char *__tmp_string_1143="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_1143),(void *)(__tmp_string_1143+3));
char *__tmp_string_1142="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_1142),(void *)(__tmp_string_1142+3));
char *__tmp_string_1141="life";
 __boundcheck_metadata_store((void *)(__tmp_string_1141),(void *)(__tmp_string_1141+4));
char *__tmp_string_1140="what";
 __boundcheck_metadata_store((void *)(__tmp_string_1140),(void *)(__tmp_string_1140+4));
char *__tmp_string_1139="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_1139),(void *)(__tmp_string_1139+6));
char *__tmp_string_1138="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_1138),(void *)(__tmp_string_1138+2));
char *__tmp_string_1137="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_1137),(void *)(__tmp_string_1137+3));
char *__tmp_string_1136="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_1136),(void *)(__tmp_string_1136+3));
char *__tmp_string_1135="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_1135),(void *)(__tmp_string_1135+7));
char *__tmp_string_1134="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_1134),(void *)(__tmp_string_1134+6));
char *__tmp_string_1133="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1133),(void *)(__tmp_string_1133+8));
char *__tmp_string_1132="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1132),(void *)(__tmp_string_1132+3));
char *__tmp_string_1131="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_1131),(void *)(__tmp_string_1131+2));
char *__tmp_string_1130="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1130),(void *)(__tmp_string_1130+3));
char *__tmp_string_1129="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1129),(void *)(__tmp_string_1129+3));
char *__tmp_string_1128="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_1128),(void *)(__tmp_string_1128+3));
char *__tmp_string_1127="people";
 __boundcheck_metadata_store((void *)(__tmp_string_1127),(void *)(__tmp_string_1127+6));
char *__tmp_string_1126="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_1126),(void *)(__tmp_string_1126+5));
char *__tmp_string_1125="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_1125),(void *)(__tmp_string_1125+2));
char *__tmp_string_1124="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_1124),(void *)(__tmp_string_1124+4));
char *__tmp_string_1123="4";
 __boundcheck_metadata_store((void *)(__tmp_string_1123),(void *)(__tmp_string_1123+1));
char *__tmp_string_1122="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_1122),(void *)(__tmp_string_1122+4));
char *__tmp_string_1121="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_1121),(void *)(__tmp_string_1121+2));
char *__tmp_string_1120="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1120),(void *)(__tmp_string_1120+3));
char *__tmp_string_1119="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_1119),(void *)(__tmp_string_1119+2));
char *__tmp_string_1118="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1118),(void *)(__tmp_string_1118+3));
char *__tmp_string_1117="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1117),(void *)(__tmp_string_1117+3));
char *__tmp_string_1116="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_1116),(void *)(__tmp_string_1116+3));
char *__tmp_string_1115="th";
 __boundcheck_metadata_store((void *)(__tmp_string_1115),(void *)(__tmp_string_1115+2));
char *__tmp_string_1114="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_1114),(void *)(__tmp_string_1114+4));
char *__tmp_string_1113="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_1113),(void *)(__tmp_string_1113+3));
char *__tmp_string_1112="an";
 __boundcheck_metadata_store((void *)(__tmp_string_1112),(void *)(__tmp_string_1112+2));
char *__tmp_string_1111="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_1111),(void *)(__tmp_string_1111+9));
char *__tmp_string_1110="way";
 __boundcheck_metadata_store((void *)(__tmp_string_1110),(void *)(__tmp_string_1110+3));
char *__tmp_string_1109="an";
 __boundcheck_metadata_store((void *)(__tmp_string_1109),(void *)(__tmp_string_1109+2));
char *__tmp_string_1108="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_1108),(void *)(__tmp_string_1108+3));
char *__tmp_string_1107="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1107),(void *)(__tmp_string_1107+3));
char *__tmp_string_1106="20";
 __boundcheck_metadata_store((void *)(__tmp_string_1106),(void *)(__tmp_string_1106+2));
char *__tmp_string_1105="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_1105),(void *)(__tmp_string_1105+5));
char *__tmp_string_1104="and";
 __boundcheck_metadata_store((void *)(__tmp_string_1104),(void *)(__tmp_string_1104+3));
char *__tmp_string_1103="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_1103),(void *)(__tmp_string_1103+2));
char *__tmp_string_1102="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1102),(void *)(__tmp_string_1102+5));
char *__tmp_string_1101="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_1101),(void *)(__tmp_string_1101+3));
char *__tmp_string_1100="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_1100),(void *)(__tmp_string_1100+5));
char *__tmp_string_1099="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1099),(void *)(__tmp_string_1099+6));
char *__tmp_string_1098="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1098),(void *)(__tmp_string_1098+6));
char *__tmp_string_1097="where";
 __boundcheck_metadata_store((void *)(__tmp_string_1097),(void *)(__tmp_string_1097+5));
char *__tmp_string_1096="have";
 __boundcheck_metadata_store((void *)(__tmp_string_1096),(void *)(__tmp_string_1096+4));
char *__tmp_string_1095="long";
 __boundcheck_metadata_store((void *)(__tmp_string_1095),(void *)(__tmp_string_1095+4));
char *__tmp_string_1094="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1094),(void *)(__tmp_string_1094+3));
char *__tmp_string_1093="for";
 __boundcheck_metadata_store((void *)(__tmp_string_1093),(void *)(__tmp_string_1093+3));
char *__tmp_string_1092="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_1092),(void *)(__tmp_string_1092+4));
char *__tmp_string_1091="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_1091),(void *)(__tmp_string_1091+4));
char *__tmp_string_1090="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_1090),(void *)(__tmp_string_1090+4));
char *__tmp_string_1089="me";
 __boundcheck_metadata_store((void *)(__tmp_string_1089),(void *)(__tmp_string_1089+2));
char *__tmp_string_1088="more";
 __boundcheck_metadata_store((void *)(__tmp_string_1088),(void *)(__tmp_string_1088+4));
char *__tmp_string_1087="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_1087),(void *)(__tmp_string_1087+5));
char *__tmp_string_1086="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1086),(void *)(__tmp_string_1086+3));
char *__tmp_string_1085="past";
 __boundcheck_metadata_store((void *)(__tmp_string_1085),(void *)(__tmp_string_1085+4));
char *__tmp_string_1084="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_1084),(void *)(__tmp_string_1084+10));
char *__tmp_string_1083="with";
 __boundcheck_metadata_store((void *)(__tmp_string_1083),(void *)(__tmp_string_1083+4));
char *__tmp_string_1082="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_1082),(void *)(__tmp_string_1082+7));
char *__tmp_string_1081="time";
 __boundcheck_metadata_store((void *)(__tmp_string_1081),(void *)(__tmp_string_1081+4));
char *__tmp_string_1080="too";
 __boundcheck_metadata_store((void *)(__tmp_string_1080),(void *)(__tmp_string_1080+3));
char *__tmp_string_1079="one";
 __boundcheck_metadata_store((void *)(__tmp_string_1079),(void *)(__tmp_string_1079+3));
char *__tmp_string_1078="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_1078),(void *)(__tmp_string_1078+6));
char *__tmp_string_1077="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_1077),(void *)(__tmp_string_1077+5));
char *__tmp_string_1076="support";
 __boundcheck_metadata_store((void *)(__tmp_string_1076),(void *)(__tmp_string_1076+7));
char *__tmp_string_1075="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1075),(void *)(__tmp_string_1075+4));
char *__tmp_string_1074="their";
 __boundcheck_metadata_store((void *)(__tmp_string_1074),(void *)(__tmp_string_1074+5));
char *__tmp_string_1073="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_1073),(void *)(__tmp_string_1073+10));
char *__tmp_string_1072="size";
 __boundcheck_metadata_store((void *)(__tmp_string_1072),(void *)(__tmp_string_1072+4));
char *__tmp_string_1071="get";
 __boundcheck_metadata_store((void *)(__tmp_string_1071),(void *)(__tmp_string_1071+3));
char *__tmp_string_1070="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_1070),(void *)(__tmp_string_1070+11));
char *__tmp_string_1069="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_1069),(void *)(__tmp_string_1069+6));
char *__tmp_string_1068="before";
 __boundcheck_metadata_store((void *)(__tmp_string_1068),(void *)(__tmp_string_1068+6));
char *__tmp_string_1067="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_1067),(void *)(__tmp_string_1067+8));
char *__tmp_string_1066="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1066),(void *)(__tmp_string_1066+2));
char *__tmp_string_1065="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1065),(void *)(__tmp_string_1065+3));
char *__tmp_string_1064="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1064),(void *)(__tmp_string_1064+3));
char *__tmp_string_1063="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1063),(void *)(__tmp_string_1063+3));
char *__tmp_string_1062="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1062),(void *)(__tmp_string_1062+3));
char *__tmp_string_1061="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1061),(void *)(__tmp_string_1061+3));
char *__tmp_string_1060="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1060),(void *)(__tmp_string_1060+3));
char *__tmp_string_1059="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1059),(void *)(__tmp_string_1059+3));
char *__tmp_string_1058="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1058),(void *)(__tmp_string_1058+3));
char *__tmp_string_1057="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1057),(void *)(__tmp_string_1057+3));
char *__tmp_string_1056="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1056),(void *)(__tmp_string_1056+3));
char *__tmp_string_1055="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1055),(void *)(__tmp_string_1055+3));
char *__tmp_string_1054="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1054),(void *)(__tmp_string_1054+3));
char *__tmp_string_1053="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1053),(void *)(__tmp_string_1053+3));
char *__tmp_string_1052="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1052),(void *)(__tmp_string_1052+3));
char *__tmp_string_1051="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1051),(void *)(__tmp_string_1051+3));
char *__tmp_string_1050="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_1050),(void *)(__tmp_string_1050+4));
char *__tmp_string_1049="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1049),(void *)(__tmp_string_1049+6));
char *__tmp_string_1048="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1048),(void *)(__tmp_string_1048+3));
char *__tmp_string_1047="room";
 __boundcheck_metadata_store((void *)(__tmp_string_1047),(void *)(__tmp_string_1047+4));
char *__tmp_string_1046="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_1046),(void *)(__tmp_string_1046+7));
char *__tmp_string_1045="own";
 __boundcheck_metadata_store((void *)(__tmp_string_1045),(void *)(__tmp_string_1045+3));
char *__tmp_string_1044="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_1044),(void *)(__tmp_string_1044+8));
char *__tmp_string_1043="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1043),(void *)(__tmp_string_1043+3));
char *__tmp_string_1042="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1042),(void *)(__tmp_string_1042+2));
char *__tmp_string_1041="other";
 __boundcheck_metadata_store((void *)(__tmp_string_1041),(void *)(__tmp_string_1041+5));
char *__tmp_string_1040="be";
 __boundcheck_metadata_store((void *)(__tmp_string_1040),(void *)(__tmp_string_1040+2));
char *__tmp_string_1039="it";
 __boundcheck_metadata_store((void *)(__tmp_string_1039),(void *)(__tmp_string_1039+2));
char *__tmp_string_1038="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_1038),(void *)(__tmp_string_1038+5));
char *__tmp_string_1037="are";
 __boundcheck_metadata_store((void *)(__tmp_string_1037),(void *)(__tmp_string_1037+3));
char *__tmp_string_1036="either";
 __boundcheck_metadata_store((void *)(__tmp_string_1036),(void *)(__tmp_string_1036+6));
char *__tmp_string_1035="much";
 __boundcheck_metadata_store((void *)(__tmp_string_1035),(void *)(__tmp_string_1035+4));
char *__tmp_string_1034="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_1034),(void *)(__tmp_string_1034+12));
char *__tmp_string_1033="your";
 __boundcheck_metadata_store((void *)(__tmp_string_1033),(void *)(__tmp_string_1033+4));
char *__tmp_string_1032="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1032),(void *)(__tmp_string_1032+3));
char *__tmp_string_1031="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1031),(void *)(__tmp_string_1031+3));
char *__tmp_string_1030="may";
 __boundcheck_metadata_store((void *)(__tmp_string_1030),(void *)(__tmp_string_1030+3));
char *__tmp_string_1029="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1029),(void *)(__tmp_string_1029+5));
char *__tmp_string_1028="them";
 __boundcheck_metadata_store((void *)(__tmp_string_1028),(void *)(__tmp_string_1028+4));
char *__tmp_string_1027="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1027),(void *)(__tmp_string_1027+3));
char *__tmp_string_1026="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_1026),(void *)(__tmp_string_1026+3));
char *__tmp_string_1025="40";
 __boundcheck_metadata_store((void *)(__tmp_string_1025),(void *)(__tmp_string_1025+2));
char *__tmp_string_1024="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1024),(void *)(__tmp_string_1024+3));
char *__tmp_string_1023="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1023),(void *)(__tmp_string_1023+3));
char *__tmp_string_1022="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_1022),(void *)(__tmp_string_1022+3));
char *__tmp_string_1021="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_1021),(void *)(__tmp_string_1021+3));
char *__tmp_string_1020="life";
 __boundcheck_metadata_store((void *)(__tmp_string_1020),(void *)(__tmp_string_1020+4));
char *__tmp_string_1019="what";
 __boundcheck_metadata_store((void *)(__tmp_string_1019),(void *)(__tmp_string_1019+4));
char *__tmp_string_1018="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_1018),(void *)(__tmp_string_1018+6));
char *__tmp_string_1017="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_1017),(void *)(__tmp_string_1017+2));
char *__tmp_string_1016="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_1016),(void *)(__tmp_string_1016+3));
char *__tmp_string_1015="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_1015),(void *)(__tmp_string_1015+3));
char *__tmp_string_1014="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_1014),(void *)(__tmp_string_1014+7));
char *__tmp_string_1013="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_1013),(void *)(__tmp_string_1013+6));
char *__tmp_string_1012="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1012),(void *)(__tmp_string_1012+8));
char *__tmp_string_1011="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1011),(void *)(__tmp_string_1011+3));
char *__tmp_string_1010="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_1010),(void *)(__tmp_string_1010+2));
char *__tmp_string_1009="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1009),(void *)(__tmp_string_1009+3));
char *__tmp_string_1008="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_1008),(void *)(__tmp_string_1008+3));
char *__tmp_string_1007="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_1007),(void *)(__tmp_string_1007+3));
char *__tmp_string_1006="people";
 __boundcheck_metadata_store((void *)(__tmp_string_1006),(void *)(__tmp_string_1006+6));
char *__tmp_string_1005="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_1005),(void *)(__tmp_string_1005+5));
char *__tmp_string_1004="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_1004),(void *)(__tmp_string_1004+2));
char *__tmp_string_1003="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_1003),(void *)(__tmp_string_1003+4));
char *__tmp_string_1002="4";
 __boundcheck_metadata_store((void *)(__tmp_string_1002),(void *)(__tmp_string_1002+1));
char *__tmp_string_1001="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_1001),(void *)(__tmp_string_1001+4));
char *__tmp_string_1000="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_1000),(void *)(__tmp_string_1000+2));
char *__tmp_string_999="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_999),(void *)(__tmp_string_999+3));
char *__tmp_string_998="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_998),(void *)(__tmp_string_998+2));
char *__tmp_string_997="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_997),(void *)(__tmp_string_997+3));
char *__tmp_string_996="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_996),(void *)(__tmp_string_996+3));
char *__tmp_string_995="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_995),(void *)(__tmp_string_995+3));
char *__tmp_string_994="th";
 __boundcheck_metadata_store((void *)(__tmp_string_994),(void *)(__tmp_string_994+2));
char *__tmp_string_993="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_993),(void *)(__tmp_string_993+4));
char *__tmp_string_992="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_992),(void *)(__tmp_string_992+3));
char *__tmp_string_991="an";
 __boundcheck_metadata_store((void *)(__tmp_string_991),(void *)(__tmp_string_991+2));
char *__tmp_string_990="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_990),(void *)(__tmp_string_990+9));
char *__tmp_string_989="way";
 __boundcheck_metadata_store((void *)(__tmp_string_989),(void *)(__tmp_string_989+3));
char *__tmp_string_988="an";
 __boundcheck_metadata_store((void *)(__tmp_string_988),(void *)(__tmp_string_988+2));
char *__tmp_string_987="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_987),(void *)(__tmp_string_987+3));
char *__tmp_string_986="you";
 __boundcheck_metadata_store((void *)(__tmp_string_986),(void *)(__tmp_string_986+3));
char *__tmp_string_985="20";
 __boundcheck_metadata_store((void *)(__tmp_string_985),(void *)(__tmp_string_985+2));
char *__tmp_string_984="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_984),(void *)(__tmp_string_984+5));
char *__tmp_string_983="and";
 __boundcheck_metadata_store((void *)(__tmp_string_983),(void *)(__tmp_string_983+3));
char *__tmp_string_982="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_982),(void *)(__tmp_string_982+2));
char *__tmp_string_981="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_981),(void *)(__tmp_string_981+5));
char *__tmp_string_980="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_980),(void *)(__tmp_string_980+3));
char *__tmp_string_979="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_979),(void *)(__tmp_string_979+5));
char *__tmp_string_978="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_978),(void *)(__tmp_string_978+6));
char *__tmp_string_977="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_977),(void *)(__tmp_string_977+6));
char *__tmp_string_976="where";
 __boundcheck_metadata_store((void *)(__tmp_string_976),(void *)(__tmp_string_976+5));
char *__tmp_string_975="have";
 __boundcheck_metadata_store((void *)(__tmp_string_975),(void *)(__tmp_string_975+4));
char *__tmp_string_974="long";
 __boundcheck_metadata_store((void *)(__tmp_string_974),(void *)(__tmp_string_974+4));
char *__tmp_string_973="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_973),(void *)(__tmp_string_973+3));
char *__tmp_string_972="for";
 __boundcheck_metadata_store((void *)(__tmp_string_972),(void *)(__tmp_string_972+3));
char *__tmp_string_971="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_971),(void *)(__tmp_string_971+4));
char *__tmp_string_970="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_970),(void *)(__tmp_string_970+4));
char *__tmp_string_969="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_969),(void *)(__tmp_string_969+4));
char *__tmp_string_968="me";
 __boundcheck_metadata_store((void *)(__tmp_string_968),(void *)(__tmp_string_968+2));
char *__tmp_string_967="more";
 __boundcheck_metadata_store((void *)(__tmp_string_967),(void *)(__tmp_string_967+4));
char *__tmp_string_966="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_966),(void *)(__tmp_string_966+5));
char *__tmp_string_965="the";
 __boundcheck_metadata_store((void *)(__tmp_string_965),(void *)(__tmp_string_965+3));
char *__tmp_string_964="past";
 __boundcheck_metadata_store((void *)(__tmp_string_964),(void *)(__tmp_string_964+4));
char *__tmp_string_963="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_963),(void *)(__tmp_string_963+10));
char *__tmp_string_962="with";
 __boundcheck_metadata_store((void *)(__tmp_string_962),(void *)(__tmp_string_962+4));
char *__tmp_string_961="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_961),(void *)(__tmp_string_961+7));
char *__tmp_string_960="time";
 __boundcheck_metadata_store((void *)(__tmp_string_960),(void *)(__tmp_string_960+4));
char *__tmp_string_959="too";
 __boundcheck_metadata_store((void *)(__tmp_string_959),(void *)(__tmp_string_959+3));
char *__tmp_string_958="one";
 __boundcheck_metadata_store((void *)(__tmp_string_958),(void *)(__tmp_string_958+3));
char *__tmp_string_957="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_957),(void *)(__tmp_string_957+6));
char *__tmp_string_956="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_956),(void *)(__tmp_string_956+5));
char *__tmp_string_955="support";
 __boundcheck_metadata_store((void *)(__tmp_string_955),(void *)(__tmp_string_955+7));
char *__tmp_string_954="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_954),(void *)(__tmp_string_954+4));
char *__tmp_string_953="their";
 __boundcheck_metadata_store((void *)(__tmp_string_953),(void *)(__tmp_string_953+5));
char *__tmp_string_952="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_952),(void *)(__tmp_string_952+10));
char *__tmp_string_951="size";
 __boundcheck_metadata_store((void *)(__tmp_string_951),(void *)(__tmp_string_951+4));
char *__tmp_string_950="get";
 __boundcheck_metadata_store((void *)(__tmp_string_950),(void *)(__tmp_string_950+3));
char *__tmp_string_949="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_949),(void *)(__tmp_string_949+11));
char *__tmp_string_948="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_948),(void *)(__tmp_string_948+6));
char *__tmp_string_947="before";
 __boundcheck_metadata_store((void *)(__tmp_string_947),(void *)(__tmp_string_947+6));
char *__tmp_string_946="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_946),(void *)(__tmp_string_946+8));
char *__tmp_string_945="it";
 __boundcheck_metadata_store((void *)(__tmp_string_945),(void *)(__tmp_string_945+2));
char *__tmp_string_944="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_944),(void *)(__tmp_string_944+3));
char *__tmp_string_943="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_943),(void *)(__tmp_string_943+3));
char *__tmp_string_942="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_942),(void *)(__tmp_string_942+3));
char *__tmp_string_941="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_941),(void *)(__tmp_string_941+3));
char *__tmp_string_940="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_940),(void *)(__tmp_string_940+3));
char *__tmp_string_939="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_939),(void *)(__tmp_string_939+3));
char *__tmp_string_938="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_938),(void *)(__tmp_string_938+3));
char *__tmp_string_937="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_937),(void *)(__tmp_string_937+3));
char *__tmp_string_936="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_936),(void *)(__tmp_string_936+3));
char *__tmp_string_935="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_935),(void *)(__tmp_string_935+3));
char *__tmp_string_934="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_934),(void *)(__tmp_string_934+3));
char *__tmp_string_933="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_933),(void *)(__tmp_string_933+3));
char *__tmp_string_932="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_932),(void *)(__tmp_string_932+3));
char *__tmp_string_931="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_931),(void *)(__tmp_string_931+3));
char *__tmp_string_930="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_930),(void *)(__tmp_string_930+3));
char *__tmp_string_929="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_929),(void *)(__tmp_string_929+4));
char *__tmp_string_928="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_928),(void *)(__tmp_string_928+6));
char *__tmp_string_927="you";
 __boundcheck_metadata_store((void *)(__tmp_string_927),(void *)(__tmp_string_927+3));
char *__tmp_string_926="room";
 __boundcheck_metadata_store((void *)(__tmp_string_926),(void *)(__tmp_string_926+4));
char *__tmp_string_925="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_925),(void *)(__tmp_string_925+7));
char *__tmp_string_924="own";
 __boundcheck_metadata_store((void *)(__tmp_string_924),(void *)(__tmp_string_924+3));
char *__tmp_string_923="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_923),(void *)(__tmp_string_923+8));
char *__tmp_string_922="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_922),(void *)(__tmp_string_922+3));
char *__tmp_string_921="it";
 __boundcheck_metadata_store((void *)(__tmp_string_921),(void *)(__tmp_string_921+2));
char *__tmp_string_920="other";
 __boundcheck_metadata_store((void *)(__tmp_string_920),(void *)(__tmp_string_920+5));
char *__tmp_string_919="be";
 __boundcheck_metadata_store((void *)(__tmp_string_919),(void *)(__tmp_string_919+2));
char *__tmp_string_918="it";
 __boundcheck_metadata_store((void *)(__tmp_string_918),(void *)(__tmp_string_918+2));
char *__tmp_string_917="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_917),(void *)(__tmp_string_917+5));
char *__tmp_string_916="are";
 __boundcheck_metadata_store((void *)(__tmp_string_916),(void *)(__tmp_string_916+3));
char *__tmp_string_915="either";
 __boundcheck_metadata_store((void *)(__tmp_string_915),(void *)(__tmp_string_915+6));
char *__tmp_string_914="much";
 __boundcheck_metadata_store((void *)(__tmp_string_914),(void *)(__tmp_string_914+4));
char *__tmp_string_913="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_913),(void *)(__tmp_string_913+12));
char *__tmp_string_912="your";
 __boundcheck_metadata_store((void *)(__tmp_string_912),(void *)(__tmp_string_912+4));
char *__tmp_string_911="the";
 __boundcheck_metadata_store((void *)(__tmp_string_911),(void *)(__tmp_string_911+3));
char *__tmp_string_910="you";
 __boundcheck_metadata_store((void *)(__tmp_string_910),(void *)(__tmp_string_910+3));
char *__tmp_string_909="may";
 __boundcheck_metadata_store((void *)(__tmp_string_909),(void *)(__tmp_string_909+3));
char *__tmp_string_908="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_908),(void *)(__tmp_string_908+5));
char *__tmp_string_907="them";
 __boundcheck_metadata_store((void *)(__tmp_string_907),(void *)(__tmp_string_907+4));
char *__tmp_string_906="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_906),(void *)(__tmp_string_906+3));
char *__tmp_string_905="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_905),(void *)(__tmp_string_905+3));
char *__tmp_string_904="40";
 __boundcheck_metadata_store((void *)(__tmp_string_904),(void *)(__tmp_string_904+2));
char *__tmp_string_903="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_903),(void *)(__tmp_string_903+3));
char *__tmp_string_902="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_902),(void *)(__tmp_string_902+3));
char *__tmp_string_901="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_901),(void *)(__tmp_string_901+3));
char *__tmp_string_900="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_900),(void *)(__tmp_string_900+3));
char *__tmp_string_899="life";
 __boundcheck_metadata_store((void *)(__tmp_string_899),(void *)(__tmp_string_899+4));
char *__tmp_string_898="what";
 __boundcheck_metadata_store((void *)(__tmp_string_898),(void *)(__tmp_string_898+4));
char *__tmp_string_897="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_897),(void *)(__tmp_string_897+6));
char *__tmp_string_896="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_896),(void *)(__tmp_string_896+2));
char *__tmp_string_895="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_895),(void *)(__tmp_string_895+3));
char *__tmp_string_894="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_894),(void *)(__tmp_string_894+3));
char *__tmp_string_893="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_893),(void *)(__tmp_string_893+7));
char *__tmp_string_892="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_892),(void *)(__tmp_string_892+6));
char *__tmp_string_891="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_891),(void *)(__tmp_string_891+8));
char *__tmp_string_890="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_890),(void *)(__tmp_string_890+3));
char *__tmp_string_889="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_889),(void *)(__tmp_string_889+2));
char *__tmp_string_888="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_888),(void *)(__tmp_string_888+3));
char *__tmp_string_887="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_887),(void *)(__tmp_string_887+3));
char *__tmp_string_886="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_886),(void *)(__tmp_string_886+3));
char *__tmp_string_885="people";
 __boundcheck_metadata_store((void *)(__tmp_string_885),(void *)(__tmp_string_885+6));
char *__tmp_string_884="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_884),(void *)(__tmp_string_884+5));
char *__tmp_string_883="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_883),(void *)(__tmp_string_883+2));
char *__tmp_string_882="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_882),(void *)(__tmp_string_882+4));
char *__tmp_string_881="4";
 __boundcheck_metadata_store((void *)(__tmp_string_881),(void *)(__tmp_string_881+1));
char *__tmp_string_880="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_880),(void *)(__tmp_string_880+4));
char *__tmp_string_879="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_879),(void *)(__tmp_string_879+2));
char *__tmp_string_878="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_878),(void *)(__tmp_string_878+3));
char *__tmp_string_877="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_877),(void *)(__tmp_string_877+2));
char *__tmp_string_876="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_876),(void *)(__tmp_string_876+3));
char *__tmp_string_875="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_875),(void *)(__tmp_string_875+3));
char *__tmp_string_874="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_874),(void *)(__tmp_string_874+3));
char *__tmp_string_873="th";
 __boundcheck_metadata_store((void *)(__tmp_string_873),(void *)(__tmp_string_873+2));
char *__tmp_string_872="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_872),(void *)(__tmp_string_872+4));
char *__tmp_string_871="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_871),(void *)(__tmp_string_871+3));
char *__tmp_string_870="an";
 __boundcheck_metadata_store((void *)(__tmp_string_870),(void *)(__tmp_string_870+2));
char *__tmp_string_869="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_869),(void *)(__tmp_string_869+9));
char *__tmp_string_868="way";
 __boundcheck_metadata_store((void *)(__tmp_string_868),(void *)(__tmp_string_868+3));
char *__tmp_string_867="an";
 __boundcheck_metadata_store((void *)(__tmp_string_867),(void *)(__tmp_string_867+2));
char *__tmp_string_866="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_866),(void *)(__tmp_string_866+3));
char *__tmp_string_865="you";
 __boundcheck_metadata_store((void *)(__tmp_string_865),(void *)(__tmp_string_865+3));
char *__tmp_string_864="20";
 __boundcheck_metadata_store((void *)(__tmp_string_864),(void *)(__tmp_string_864+2));
char *__tmp_string_863="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_863),(void *)(__tmp_string_863+5));
char *__tmp_string_862="and";
 __boundcheck_metadata_store((void *)(__tmp_string_862),(void *)(__tmp_string_862+3));
char *__tmp_string_861="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_861),(void *)(__tmp_string_861+2));
char *__tmp_string_860="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_860),(void *)(__tmp_string_860+5));
char *__tmp_string_859="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_859),(void *)(__tmp_string_859+3));
char *__tmp_string_858="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_858),(void *)(__tmp_string_858+5));
char *__tmp_string_857="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_857),(void *)(__tmp_string_857+6));
char *__tmp_string_856="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_856),(void *)(__tmp_string_856+6));
char *__tmp_string_855="where";
 __boundcheck_metadata_store((void *)(__tmp_string_855),(void *)(__tmp_string_855+5));
char *__tmp_string_854="have";
 __boundcheck_metadata_store((void *)(__tmp_string_854),(void *)(__tmp_string_854+4));
char *__tmp_string_853="long";
 __boundcheck_metadata_store((void *)(__tmp_string_853),(void *)(__tmp_string_853+4));
char *__tmp_string_852="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_852),(void *)(__tmp_string_852+3));
char *__tmp_string_851="for";
 __boundcheck_metadata_store((void *)(__tmp_string_851),(void *)(__tmp_string_851+3));
char *__tmp_string_850="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_850),(void *)(__tmp_string_850+4));
char *__tmp_string_849="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_849),(void *)(__tmp_string_849+4));
char *__tmp_string_848="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_848),(void *)(__tmp_string_848+4));
char *__tmp_string_847="me";
 __boundcheck_metadata_store((void *)(__tmp_string_847),(void *)(__tmp_string_847+2));
char *__tmp_string_846="more";
 __boundcheck_metadata_store((void *)(__tmp_string_846),(void *)(__tmp_string_846+4));
char *__tmp_string_845="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_845),(void *)(__tmp_string_845+5));
char *__tmp_string_844="the";
 __boundcheck_metadata_store((void *)(__tmp_string_844),(void *)(__tmp_string_844+3));
char *__tmp_string_843="past";
 __boundcheck_metadata_store((void *)(__tmp_string_843),(void *)(__tmp_string_843+4));
char *__tmp_string_842="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_842),(void *)(__tmp_string_842+10));
char *__tmp_string_841="with";
 __boundcheck_metadata_store((void *)(__tmp_string_841),(void *)(__tmp_string_841+4));
char *__tmp_string_840="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_840),(void *)(__tmp_string_840+7));
char *__tmp_string_839="time";
 __boundcheck_metadata_store((void *)(__tmp_string_839),(void *)(__tmp_string_839+4));
char *__tmp_string_838="too";
 __boundcheck_metadata_store((void *)(__tmp_string_838),(void *)(__tmp_string_838+3));
char *__tmp_string_837="one";
 __boundcheck_metadata_store((void *)(__tmp_string_837),(void *)(__tmp_string_837+3));
char *__tmp_string_836="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_836),(void *)(__tmp_string_836+6));
char *__tmp_string_835="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_835),(void *)(__tmp_string_835+5));
char *__tmp_string_834="support";
 __boundcheck_metadata_store((void *)(__tmp_string_834),(void *)(__tmp_string_834+7));
char *__tmp_string_833="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_833),(void *)(__tmp_string_833+4));
char *__tmp_string_832="their";
 __boundcheck_metadata_store((void *)(__tmp_string_832),(void *)(__tmp_string_832+5));
char *__tmp_string_831="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_831),(void *)(__tmp_string_831+10));
char *__tmp_string_830="size";
 __boundcheck_metadata_store((void *)(__tmp_string_830),(void *)(__tmp_string_830+4));
char *__tmp_string_829="get";
 __boundcheck_metadata_store((void *)(__tmp_string_829),(void *)(__tmp_string_829+3));
char *__tmp_string_828="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_828),(void *)(__tmp_string_828+11));
char *__tmp_string_827="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_827),(void *)(__tmp_string_827+6));
char *__tmp_string_826="before";
 __boundcheck_metadata_store((void *)(__tmp_string_826),(void *)(__tmp_string_826+6));
char *__tmp_string_825="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_825),(void *)(__tmp_string_825+8));
char *__tmp_string_824="it";
 __boundcheck_metadata_store((void *)(__tmp_string_824),(void *)(__tmp_string_824+2));
char *__tmp_string_823="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_823),(void *)(__tmp_string_823+3));
char *__tmp_string_822="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_822),(void *)(__tmp_string_822+3));
char *__tmp_string_821="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_821),(void *)(__tmp_string_821+3));
char *__tmp_string_820="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_820),(void *)(__tmp_string_820+3));
char *__tmp_string_819="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_819),(void *)(__tmp_string_819+3));
char *__tmp_string_818="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_818),(void *)(__tmp_string_818+3));
char *__tmp_string_817="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_817),(void *)(__tmp_string_817+3));
char *__tmp_string_816="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_816),(void *)(__tmp_string_816+3));
char *__tmp_string_815="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_815),(void *)(__tmp_string_815+3));
char *__tmp_string_814="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_814),(void *)(__tmp_string_814+3));
char *__tmp_string_813="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_813),(void *)(__tmp_string_813+3));
char *__tmp_string_812="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_812),(void *)(__tmp_string_812+3));
char *__tmp_string_811="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_811),(void *)(__tmp_string_811+3));
char *__tmp_string_810="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_810),(void *)(__tmp_string_810+3));
char *__tmp_string_809="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_809),(void *)(__tmp_string_809+3));
char *__tmp_string_808="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_808),(void *)(__tmp_string_808+4));
char *__tmp_string_807="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_807),(void *)(__tmp_string_807+6));
char *__tmp_string_806="you";
 __boundcheck_metadata_store((void *)(__tmp_string_806),(void *)(__tmp_string_806+3));
char *__tmp_string_805="room";
 __boundcheck_metadata_store((void *)(__tmp_string_805),(void *)(__tmp_string_805+4));
char *__tmp_string_804="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_804),(void *)(__tmp_string_804+7));
char *__tmp_string_803="own";
 __boundcheck_metadata_store((void *)(__tmp_string_803),(void *)(__tmp_string_803+3));
char *__tmp_string_802="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_802),(void *)(__tmp_string_802+8));
char *__tmp_string_801="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_801),(void *)(__tmp_string_801+3));
char *__tmp_string_800="it";
 __boundcheck_metadata_store((void *)(__tmp_string_800),(void *)(__tmp_string_800+2));
char *__tmp_string_799="other";
 __boundcheck_metadata_store((void *)(__tmp_string_799),(void *)(__tmp_string_799+5));
char *__tmp_string_798="be";
 __boundcheck_metadata_store((void *)(__tmp_string_798),(void *)(__tmp_string_798+2));
char *__tmp_string_797="it";
 __boundcheck_metadata_store((void *)(__tmp_string_797),(void *)(__tmp_string_797+2));
char *__tmp_string_796="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_796),(void *)(__tmp_string_796+5));
char *__tmp_string_795="are";
 __boundcheck_metadata_store((void *)(__tmp_string_795),(void *)(__tmp_string_795+3));
char *__tmp_string_794="either";
 __boundcheck_metadata_store((void *)(__tmp_string_794),(void *)(__tmp_string_794+6));
char *__tmp_string_793="much";
 __boundcheck_metadata_store((void *)(__tmp_string_793),(void *)(__tmp_string_793+4));
char *__tmp_string_792="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_792),(void *)(__tmp_string_792+12));
char *__tmp_string_791="your";
 __boundcheck_metadata_store((void *)(__tmp_string_791),(void *)(__tmp_string_791+4));
char *__tmp_string_790="the";
 __boundcheck_metadata_store((void *)(__tmp_string_790),(void *)(__tmp_string_790+3));
char *__tmp_string_789="you";
 __boundcheck_metadata_store((void *)(__tmp_string_789),(void *)(__tmp_string_789+3));
char *__tmp_string_788="may";
 __boundcheck_metadata_store((void *)(__tmp_string_788),(void *)(__tmp_string_788+3));
char *__tmp_string_787="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_787),(void *)(__tmp_string_787+5));
char *__tmp_string_786="them";
 __boundcheck_metadata_store((void *)(__tmp_string_786),(void *)(__tmp_string_786+4));
char *__tmp_string_785="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_785),(void *)(__tmp_string_785+3));
char *__tmp_string_784="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_784),(void *)(__tmp_string_784+3));
char *__tmp_string_783="40";
 __boundcheck_metadata_store((void *)(__tmp_string_783),(void *)(__tmp_string_783+2));
char *__tmp_string_782="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_782),(void *)(__tmp_string_782+3));
char *__tmp_string_781="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_781),(void *)(__tmp_string_781+3));
char *__tmp_string_780="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_780),(void *)(__tmp_string_780+3));
char *__tmp_string_779="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_779),(void *)(__tmp_string_779+3));
char *__tmp_string_778="life";
 __boundcheck_metadata_store((void *)(__tmp_string_778),(void *)(__tmp_string_778+4));
char *__tmp_string_777="what";
 __boundcheck_metadata_store((void *)(__tmp_string_777),(void *)(__tmp_string_777+4));
char *__tmp_string_776="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_776),(void *)(__tmp_string_776+6));
char *__tmp_string_775="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_775),(void *)(__tmp_string_775+2));
char *__tmp_string_774="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_774),(void *)(__tmp_string_774+3));
char *__tmp_string_773="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_773),(void *)(__tmp_string_773+3));
char *__tmp_string_772="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_772),(void *)(__tmp_string_772+7));
char *__tmp_string_771="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_771),(void *)(__tmp_string_771+6));
char *__tmp_string_770="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_770),(void *)(__tmp_string_770+8));
char *__tmp_string_769="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_769),(void *)(__tmp_string_769+3));
char *__tmp_string_768="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_768),(void *)(__tmp_string_768+2));
char *__tmp_string_767="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_767),(void *)(__tmp_string_767+3));
char *__tmp_string_766="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_766),(void *)(__tmp_string_766+3));
char *__tmp_string_765="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_765),(void *)(__tmp_string_765+3));
char *__tmp_string_764="people";
 __boundcheck_metadata_store((void *)(__tmp_string_764),(void *)(__tmp_string_764+6));
char *__tmp_string_763="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_763),(void *)(__tmp_string_763+5));
char *__tmp_string_762="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_762),(void *)(__tmp_string_762+2));
char *__tmp_string_761="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_761),(void *)(__tmp_string_761+4));
char *__tmp_string_760="4";
 __boundcheck_metadata_store((void *)(__tmp_string_760),(void *)(__tmp_string_760+1));
char *__tmp_string_759="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_759),(void *)(__tmp_string_759+4));
char *__tmp_string_758="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_758),(void *)(__tmp_string_758+2));
char *__tmp_string_757="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_757),(void *)(__tmp_string_757+3));
char *__tmp_string_756="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_756),(void *)(__tmp_string_756+2));
char *__tmp_string_755="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_755),(void *)(__tmp_string_755+3));
char *__tmp_string_754="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_754),(void *)(__tmp_string_754+3));
char *__tmp_string_753="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_753),(void *)(__tmp_string_753+3));
char *__tmp_string_752="th";
 __boundcheck_metadata_store((void *)(__tmp_string_752),(void *)(__tmp_string_752+2));
char *__tmp_string_751="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_751),(void *)(__tmp_string_751+4));
char *__tmp_string_750="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_750),(void *)(__tmp_string_750+3));
char *__tmp_string_749="an";
 __boundcheck_metadata_store((void *)(__tmp_string_749),(void *)(__tmp_string_749+2));
char *__tmp_string_748="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_748),(void *)(__tmp_string_748+9));
char *__tmp_string_747="way";
 __boundcheck_metadata_store((void *)(__tmp_string_747),(void *)(__tmp_string_747+3));
char *__tmp_string_746="an";
 __boundcheck_metadata_store((void *)(__tmp_string_746),(void *)(__tmp_string_746+2));
char *__tmp_string_745="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_745),(void *)(__tmp_string_745+3));
char *__tmp_string_744="you";
 __boundcheck_metadata_store((void *)(__tmp_string_744),(void *)(__tmp_string_744+3));
char *__tmp_string_743="20";
 __boundcheck_metadata_store((void *)(__tmp_string_743),(void *)(__tmp_string_743+2));
char *__tmp_string_742="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_742),(void *)(__tmp_string_742+5));
char *__tmp_string_741="and";
 __boundcheck_metadata_store((void *)(__tmp_string_741),(void *)(__tmp_string_741+3));
char *__tmp_string_740="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_740),(void *)(__tmp_string_740+2));
char *__tmp_string_739="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_739),(void *)(__tmp_string_739+5));
char *__tmp_string_738="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_738),(void *)(__tmp_string_738+3));
char *__tmp_string_737="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_737),(void *)(__tmp_string_737+5));
char *__tmp_string_736="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_736),(void *)(__tmp_string_736+6));
char *__tmp_string_735="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_735),(void *)(__tmp_string_735+6));
char *__tmp_string_734="where";
 __boundcheck_metadata_store((void *)(__tmp_string_734),(void *)(__tmp_string_734+5));
char *__tmp_string_733="have";
 __boundcheck_metadata_store((void *)(__tmp_string_733),(void *)(__tmp_string_733+4));
char *__tmp_string_732="long";
 __boundcheck_metadata_store((void *)(__tmp_string_732),(void *)(__tmp_string_732+4));
char *__tmp_string_731="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_731),(void *)(__tmp_string_731+3));
char *__tmp_string_730="for";
 __boundcheck_metadata_store((void *)(__tmp_string_730),(void *)(__tmp_string_730+3));
char *__tmp_string_729="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_729),(void *)(__tmp_string_729+4));
char *__tmp_string_728="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_728),(void *)(__tmp_string_728+4));
char *__tmp_string_727="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_727),(void *)(__tmp_string_727+4));
char *__tmp_string_726="me";
 __boundcheck_metadata_store((void *)(__tmp_string_726),(void *)(__tmp_string_726+2));
char *__tmp_string_725="more";
 __boundcheck_metadata_store((void *)(__tmp_string_725),(void *)(__tmp_string_725+4));
char *__tmp_string_724="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_724),(void *)(__tmp_string_724+5));
char *__tmp_string_723="the";
 __boundcheck_metadata_store((void *)(__tmp_string_723),(void *)(__tmp_string_723+3));
char *__tmp_string_722="past";
 __boundcheck_metadata_store((void *)(__tmp_string_722),(void *)(__tmp_string_722+4));
char *__tmp_string_721="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_721),(void *)(__tmp_string_721+10));
char *__tmp_string_720="with";
 __boundcheck_metadata_store((void *)(__tmp_string_720),(void *)(__tmp_string_720+4));
char *__tmp_string_719="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_719),(void *)(__tmp_string_719+7));
char *__tmp_string_718="time";
 __boundcheck_metadata_store((void *)(__tmp_string_718),(void *)(__tmp_string_718+4));
char *__tmp_string_717="too";
 __boundcheck_metadata_store((void *)(__tmp_string_717),(void *)(__tmp_string_717+3));
char *__tmp_string_716="one";
 __boundcheck_metadata_store((void *)(__tmp_string_716),(void *)(__tmp_string_716+3));
char *__tmp_string_715="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_715),(void *)(__tmp_string_715+6));
char *__tmp_string_714="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_714),(void *)(__tmp_string_714+5));
char *__tmp_string_713="support";
 __boundcheck_metadata_store((void *)(__tmp_string_713),(void *)(__tmp_string_713+7));
char *__tmp_string_712="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_712),(void *)(__tmp_string_712+4));
char *__tmp_string_711="their";
 __boundcheck_metadata_store((void *)(__tmp_string_711),(void *)(__tmp_string_711+5));
char *__tmp_string_710="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_710),(void *)(__tmp_string_710+10));
char *__tmp_string_709="size";
 __boundcheck_metadata_store((void *)(__tmp_string_709),(void *)(__tmp_string_709+4));
char *__tmp_string_708="get";
 __boundcheck_metadata_store((void *)(__tmp_string_708),(void *)(__tmp_string_708+3));
char *__tmp_string_707="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_707),(void *)(__tmp_string_707+11));
char *__tmp_string_706="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_706),(void *)(__tmp_string_706+6));
char *__tmp_string_705="before";
 __boundcheck_metadata_store((void *)(__tmp_string_705),(void *)(__tmp_string_705+6));
char *__tmp_string_704="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_704),(void *)(__tmp_string_704+8));
char *__tmp_string_703="it";
 __boundcheck_metadata_store((void *)(__tmp_string_703),(void *)(__tmp_string_703+2));
char *__tmp_string_702="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_702),(void *)(__tmp_string_702+3));
char *__tmp_string_701="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_701),(void *)(__tmp_string_701+3));
char *__tmp_string_700="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_700),(void *)(__tmp_string_700+3));
char *__tmp_string_699="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_699),(void *)(__tmp_string_699+3));
char *__tmp_string_698="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_698),(void *)(__tmp_string_698+3));
char *__tmp_string_697="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_697),(void *)(__tmp_string_697+3));
char *__tmp_string_696="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_696),(void *)(__tmp_string_696+3));
char *__tmp_string_695="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_695),(void *)(__tmp_string_695+3));
char *__tmp_string_694="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_694),(void *)(__tmp_string_694+3));
char *__tmp_string_693="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_693),(void *)(__tmp_string_693+3));
char *__tmp_string_692="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_692),(void *)(__tmp_string_692+3));
char *__tmp_string_691="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_691),(void *)(__tmp_string_691+3));
char *__tmp_string_690="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_690),(void *)(__tmp_string_690+3));
char *__tmp_string_689="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_689),(void *)(__tmp_string_689+3));
char *__tmp_string_688="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_688),(void *)(__tmp_string_688+3));
char *__tmp_string_687="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_687),(void *)(__tmp_string_687+4));
char *__tmp_string_686="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_686),(void *)(__tmp_string_686+6));
char *__tmp_string_685="you";
 __boundcheck_metadata_store((void *)(__tmp_string_685),(void *)(__tmp_string_685+3));
char *__tmp_string_684="room";
 __boundcheck_metadata_store((void *)(__tmp_string_684),(void *)(__tmp_string_684+4));
char *__tmp_string_683="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_683),(void *)(__tmp_string_683+7));
char *__tmp_string_682="own";
 __boundcheck_metadata_store((void *)(__tmp_string_682),(void *)(__tmp_string_682+3));
char *__tmp_string_681="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_681),(void *)(__tmp_string_681+8));
char *__tmp_string_680="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_680),(void *)(__tmp_string_680+3));
char *__tmp_string_679="it";
 __boundcheck_metadata_store((void *)(__tmp_string_679),(void *)(__tmp_string_679+2));
char *__tmp_string_678="other";
 __boundcheck_metadata_store((void *)(__tmp_string_678),(void *)(__tmp_string_678+5));
char *__tmp_string_677="be";
 __boundcheck_metadata_store((void *)(__tmp_string_677),(void *)(__tmp_string_677+2));
char *__tmp_string_676="it";
 __boundcheck_metadata_store((void *)(__tmp_string_676),(void *)(__tmp_string_676+2));
char *__tmp_string_675="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_675),(void *)(__tmp_string_675+5));
char *__tmp_string_674="are";
 __boundcheck_metadata_store((void *)(__tmp_string_674),(void *)(__tmp_string_674+3));
char *__tmp_string_673="either";
 __boundcheck_metadata_store((void *)(__tmp_string_673),(void *)(__tmp_string_673+6));
char *__tmp_string_672="much";
 __boundcheck_metadata_store((void *)(__tmp_string_672),(void *)(__tmp_string_672+4));
char *__tmp_string_671="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_671),(void *)(__tmp_string_671+12));
char *__tmp_string_670="your";
 __boundcheck_metadata_store((void *)(__tmp_string_670),(void *)(__tmp_string_670+4));
char *__tmp_string_669="the";
 __boundcheck_metadata_store((void *)(__tmp_string_669),(void *)(__tmp_string_669+3));
char *__tmp_string_668="you";
 __boundcheck_metadata_store((void *)(__tmp_string_668),(void *)(__tmp_string_668+3));
char *__tmp_string_667="may";
 __boundcheck_metadata_store((void *)(__tmp_string_667),(void *)(__tmp_string_667+3));
char *__tmp_string_666="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_666),(void *)(__tmp_string_666+5));
char *__tmp_string_665="them";
 __boundcheck_metadata_store((void *)(__tmp_string_665),(void *)(__tmp_string_665+4));
char *__tmp_string_664="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_664),(void *)(__tmp_string_664+3));
char *__tmp_string_663="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_663),(void *)(__tmp_string_663+3));
char *__tmp_string_662="40";
 __boundcheck_metadata_store((void *)(__tmp_string_662),(void *)(__tmp_string_662+2));
char *__tmp_string_661="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_661),(void *)(__tmp_string_661+3));
char *__tmp_string_660="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_660),(void *)(__tmp_string_660+3));
char *__tmp_string_659="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_659),(void *)(__tmp_string_659+3));
char *__tmp_string_658="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_658),(void *)(__tmp_string_658+3));
char *__tmp_string_657="life";
 __boundcheck_metadata_store((void *)(__tmp_string_657),(void *)(__tmp_string_657+4));
char *__tmp_string_656="what";
 __boundcheck_metadata_store((void *)(__tmp_string_656),(void *)(__tmp_string_656+4));
char *__tmp_string_655="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_655),(void *)(__tmp_string_655+6));
char *__tmp_string_654="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_654),(void *)(__tmp_string_654+2));
char *__tmp_string_653="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_653),(void *)(__tmp_string_653+3));
char *__tmp_string_652="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_652),(void *)(__tmp_string_652+3));
char *__tmp_string_651="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_651),(void *)(__tmp_string_651+7));
char *__tmp_string_650="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_650),(void *)(__tmp_string_650+6));
char *__tmp_string_649="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_649),(void *)(__tmp_string_649+8));
char *__tmp_string_648="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_648),(void *)(__tmp_string_648+3));
char *__tmp_string_647="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_647),(void *)(__tmp_string_647+2));
char *__tmp_string_646="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_646),(void *)(__tmp_string_646+3));
char *__tmp_string_645="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_645),(void *)(__tmp_string_645+3));
char *__tmp_string_644="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_644),(void *)(__tmp_string_644+3));
char *__tmp_string_643="people";
 __boundcheck_metadata_store((void *)(__tmp_string_643),(void *)(__tmp_string_643+6));
char *__tmp_string_642="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_642),(void *)(__tmp_string_642+5));
char *__tmp_string_641="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_641),(void *)(__tmp_string_641+2));
char *__tmp_string_640="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_640),(void *)(__tmp_string_640+4));
char *__tmp_string_639="4";
 __boundcheck_metadata_store((void *)(__tmp_string_639),(void *)(__tmp_string_639+1));
char *__tmp_string_638="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_638),(void *)(__tmp_string_638+4));
char *__tmp_string_637="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_637),(void *)(__tmp_string_637+2));
char *__tmp_string_636="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_636),(void *)(__tmp_string_636+3));
char *__tmp_string_635="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_635),(void *)(__tmp_string_635+2));
char *__tmp_string_634="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_634),(void *)(__tmp_string_634+3));
char *__tmp_string_633="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_633),(void *)(__tmp_string_633+3));
char *__tmp_string_632="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_632),(void *)(__tmp_string_632+3));
char *__tmp_string_631="th";
 __boundcheck_metadata_store((void *)(__tmp_string_631),(void *)(__tmp_string_631+2));
char *__tmp_string_630="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_630),(void *)(__tmp_string_630+4));
char *__tmp_string_629="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_629),(void *)(__tmp_string_629+3));
char *__tmp_string_628="an";
 __boundcheck_metadata_store((void *)(__tmp_string_628),(void *)(__tmp_string_628+2));
char *__tmp_string_627="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_627),(void *)(__tmp_string_627+9));
char *__tmp_string_626="way";
 __boundcheck_metadata_store((void *)(__tmp_string_626),(void *)(__tmp_string_626+3));
char *__tmp_string_625="an";
 __boundcheck_metadata_store((void *)(__tmp_string_625),(void *)(__tmp_string_625+2));
char *__tmp_string_624="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_624),(void *)(__tmp_string_624+3));
char *__tmp_string_623="you";
 __boundcheck_metadata_store((void *)(__tmp_string_623),(void *)(__tmp_string_623+3));
char *__tmp_string_622="20";
 __boundcheck_metadata_store((void *)(__tmp_string_622),(void *)(__tmp_string_622+2));
char *__tmp_string_621="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_621),(void *)(__tmp_string_621+5));
char *__tmp_string_620="and";
 __boundcheck_metadata_store((void *)(__tmp_string_620),(void *)(__tmp_string_620+3));
char *__tmp_string_619="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_619),(void *)(__tmp_string_619+2));
char *__tmp_string_618="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_618),(void *)(__tmp_string_618+5));
char *__tmp_string_617="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_617),(void *)(__tmp_string_617+3));
char *__tmp_string_616="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_616),(void *)(__tmp_string_616+5));
char *__tmp_string_615="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_615),(void *)(__tmp_string_615+6));
char *__tmp_string_614="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_614),(void *)(__tmp_string_614+6));
char *__tmp_string_613="where";
 __boundcheck_metadata_store((void *)(__tmp_string_613),(void *)(__tmp_string_613+5));
char *__tmp_string_612="have";
 __boundcheck_metadata_store((void *)(__tmp_string_612),(void *)(__tmp_string_612+4));
char *__tmp_string_611="long";
 __boundcheck_metadata_store((void *)(__tmp_string_611),(void *)(__tmp_string_611+4));
char *__tmp_string_610="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_610),(void *)(__tmp_string_610+3));
char *__tmp_string_609="for";
 __boundcheck_metadata_store((void *)(__tmp_string_609),(void *)(__tmp_string_609+3));
char *__tmp_string_608="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_608),(void *)(__tmp_string_608+4));
char *__tmp_string_607="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_607),(void *)(__tmp_string_607+4));
char *__tmp_string_606="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_606),(void *)(__tmp_string_606+4));
char *__tmp_string_605="me";
 __boundcheck_metadata_store((void *)(__tmp_string_605),(void *)(__tmp_string_605+2));
char *__tmp_string_604="more";
 __boundcheck_metadata_store((void *)(__tmp_string_604),(void *)(__tmp_string_604+4));
char *__tmp_string_603="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_603),(void *)(__tmp_string_603+5));
char *__tmp_string_602="the";
 __boundcheck_metadata_store((void *)(__tmp_string_602),(void *)(__tmp_string_602+3));
char *__tmp_string_601="past";
 __boundcheck_metadata_store((void *)(__tmp_string_601),(void *)(__tmp_string_601+4));
char *__tmp_string_600="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_600),(void *)(__tmp_string_600+10));
char *__tmp_string_599="with";
 __boundcheck_metadata_store((void *)(__tmp_string_599),(void *)(__tmp_string_599+4));
char *__tmp_string_598="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_598),(void *)(__tmp_string_598+7));
char *__tmp_string_597="time";
 __boundcheck_metadata_store((void *)(__tmp_string_597),(void *)(__tmp_string_597+4));
char *__tmp_string_596="too";
 __boundcheck_metadata_store((void *)(__tmp_string_596),(void *)(__tmp_string_596+3));
char *__tmp_string_595="one";
 __boundcheck_metadata_store((void *)(__tmp_string_595),(void *)(__tmp_string_595+3));
char *__tmp_string_594="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_594),(void *)(__tmp_string_594+6));
char *__tmp_string_593="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_593),(void *)(__tmp_string_593+5));
char *__tmp_string_592="support";
 __boundcheck_metadata_store((void *)(__tmp_string_592),(void *)(__tmp_string_592+7));
char *__tmp_string_591="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_591),(void *)(__tmp_string_591+4));
char *__tmp_string_590="their";
 __boundcheck_metadata_store((void *)(__tmp_string_590),(void *)(__tmp_string_590+5));
char *__tmp_string_589="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_589),(void *)(__tmp_string_589+10));
char *__tmp_string_588="size";
 __boundcheck_metadata_store((void *)(__tmp_string_588),(void *)(__tmp_string_588+4));
char *__tmp_string_587="get";
 __boundcheck_metadata_store((void *)(__tmp_string_587),(void *)(__tmp_string_587+3));
char *__tmp_string_586="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_586),(void *)(__tmp_string_586+11));
char *__tmp_string_585="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_585),(void *)(__tmp_string_585+6));
char *__tmp_string_584="before";
 __boundcheck_metadata_store((void *)(__tmp_string_584),(void *)(__tmp_string_584+6));
char *__tmp_string_583="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_583),(void *)(__tmp_string_583+8));
char *__tmp_string_582="it";
 __boundcheck_metadata_store((void *)(__tmp_string_582),(void *)(__tmp_string_582+2));
char *__tmp_string_581="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_581),(void *)(__tmp_string_581+3));
char *__tmp_string_580="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_580),(void *)(__tmp_string_580+3));
char *__tmp_string_579="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_579),(void *)(__tmp_string_579+3));
char *__tmp_string_578="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_578),(void *)(__tmp_string_578+3));
char *__tmp_string_577="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_577),(void *)(__tmp_string_577+3));
char *__tmp_string_576="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_576),(void *)(__tmp_string_576+3));
char *__tmp_string_575="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_575),(void *)(__tmp_string_575+3));
char *__tmp_string_574="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_574),(void *)(__tmp_string_574+3));
char *__tmp_string_573="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_573),(void *)(__tmp_string_573+3));
char *__tmp_string_572="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_572),(void *)(__tmp_string_572+3));
char *__tmp_string_571="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_571),(void *)(__tmp_string_571+3));
char *__tmp_string_570="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_570),(void *)(__tmp_string_570+3));
char *__tmp_string_569="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_569),(void *)(__tmp_string_569+3));
char *__tmp_string_568="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_568),(void *)(__tmp_string_568+3));
char *__tmp_string_567="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_567),(void *)(__tmp_string_567+3));
char *__tmp_string_566="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_566),(void *)(__tmp_string_566+4));
char *__tmp_string_565="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_565),(void *)(__tmp_string_565+6));
char *__tmp_string_564="you";
 __boundcheck_metadata_store((void *)(__tmp_string_564),(void *)(__tmp_string_564+3));
char *__tmp_string_563="room";
 __boundcheck_metadata_store((void *)(__tmp_string_563),(void *)(__tmp_string_563+4));
char *__tmp_string_562="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_562),(void *)(__tmp_string_562+7));
char *__tmp_string_561="own";
 __boundcheck_metadata_store((void *)(__tmp_string_561),(void *)(__tmp_string_561+3));
char *__tmp_string_560="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_560),(void *)(__tmp_string_560+8));
char *__tmp_string_559="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_559),(void *)(__tmp_string_559+3));
char *__tmp_string_558="it";
 __boundcheck_metadata_store((void *)(__tmp_string_558),(void *)(__tmp_string_558+2));
char *__tmp_string_557="other";
 __boundcheck_metadata_store((void *)(__tmp_string_557),(void *)(__tmp_string_557+5));
char *__tmp_string_556="be";
 __boundcheck_metadata_store((void *)(__tmp_string_556),(void *)(__tmp_string_556+2));
char *__tmp_string_555="it";
 __boundcheck_metadata_store((void *)(__tmp_string_555),(void *)(__tmp_string_555+2));
char *__tmp_string_554="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_554),(void *)(__tmp_string_554+5));
char *__tmp_string_553="are";
 __boundcheck_metadata_store((void *)(__tmp_string_553),(void *)(__tmp_string_553+3));
char *__tmp_string_552="either";
 __boundcheck_metadata_store((void *)(__tmp_string_552),(void *)(__tmp_string_552+6));
char *__tmp_string_551="much";
 __boundcheck_metadata_store((void *)(__tmp_string_551),(void *)(__tmp_string_551+4));
char *__tmp_string_550="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_550),(void *)(__tmp_string_550+12));
char *__tmp_string_549="your";
 __boundcheck_metadata_store((void *)(__tmp_string_549),(void *)(__tmp_string_549+4));
char *__tmp_string_548="the";
 __boundcheck_metadata_store((void *)(__tmp_string_548),(void *)(__tmp_string_548+3));
char *__tmp_string_547="you";
 __boundcheck_metadata_store((void *)(__tmp_string_547),(void *)(__tmp_string_547+3));
char *__tmp_string_546="may";
 __boundcheck_metadata_store((void *)(__tmp_string_546),(void *)(__tmp_string_546+3));
char *__tmp_string_545="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_545),(void *)(__tmp_string_545+5));
char *__tmp_string_544="them";
 __boundcheck_metadata_store((void *)(__tmp_string_544),(void *)(__tmp_string_544+4));
char *__tmp_string_543="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_543),(void *)(__tmp_string_543+3));
char *__tmp_string_542="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_542),(void *)(__tmp_string_542+3));
char *__tmp_string_541="40";
 __boundcheck_metadata_store((void *)(__tmp_string_541),(void *)(__tmp_string_541+2));
char *__tmp_string_540="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_540),(void *)(__tmp_string_540+3));
char *__tmp_string_539="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_539),(void *)(__tmp_string_539+3));
char *__tmp_string_538="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_538),(void *)(__tmp_string_538+3));
char *__tmp_string_537="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_537),(void *)(__tmp_string_537+3));
char *__tmp_string_536="life";
 __boundcheck_metadata_store((void *)(__tmp_string_536),(void *)(__tmp_string_536+4));
char *__tmp_string_535="what";
 __boundcheck_metadata_store((void *)(__tmp_string_535),(void *)(__tmp_string_535+4));
char *__tmp_string_534="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_534),(void *)(__tmp_string_534+6));
char *__tmp_string_533="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_533),(void *)(__tmp_string_533+2));
char *__tmp_string_532="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_532),(void *)(__tmp_string_532+3));
char *__tmp_string_531="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_531),(void *)(__tmp_string_531+3));
char *__tmp_string_530="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_530),(void *)(__tmp_string_530+7));
char *__tmp_string_529="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_529),(void *)(__tmp_string_529+6));
char *__tmp_string_528="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_528),(void *)(__tmp_string_528+8));
char *__tmp_string_527="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_527),(void *)(__tmp_string_527+3));
char *__tmp_string_526="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_526),(void *)(__tmp_string_526+2));
char *__tmp_string_525="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_525),(void *)(__tmp_string_525+3));
char *__tmp_string_524="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_524),(void *)(__tmp_string_524+3));
char *__tmp_string_523="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_523),(void *)(__tmp_string_523+3));
char *__tmp_string_522="people";
 __boundcheck_metadata_store((void *)(__tmp_string_522),(void *)(__tmp_string_522+6));
char *__tmp_string_521="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_521),(void *)(__tmp_string_521+5));
char *__tmp_string_520="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_520),(void *)(__tmp_string_520+2));
char *__tmp_string_519="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_519),(void *)(__tmp_string_519+4));
char *__tmp_string_518="4";
 __boundcheck_metadata_store((void *)(__tmp_string_518),(void *)(__tmp_string_518+1));
char *__tmp_string_517="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_517),(void *)(__tmp_string_517+4));
char *__tmp_string_516="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_516),(void *)(__tmp_string_516+2));
char *__tmp_string_515="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_515),(void *)(__tmp_string_515+3));
char *__tmp_string_514="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_514),(void *)(__tmp_string_514+2));
char *__tmp_string_513="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_513),(void *)(__tmp_string_513+3));
char *__tmp_string_512="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_512),(void *)(__tmp_string_512+3));
char *__tmp_string_511="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_511),(void *)(__tmp_string_511+3));
char *__tmp_string_510="th";
 __boundcheck_metadata_store((void *)(__tmp_string_510),(void *)(__tmp_string_510+2));
char *__tmp_string_509="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_509),(void *)(__tmp_string_509+4));
char *__tmp_string_508="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_508),(void *)(__tmp_string_508+3));
char *__tmp_string_507="an";
 __boundcheck_metadata_store((void *)(__tmp_string_507),(void *)(__tmp_string_507+2));
char *__tmp_string_506="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_506),(void *)(__tmp_string_506+9));
char *__tmp_string_505="way";
 __boundcheck_metadata_store((void *)(__tmp_string_505),(void *)(__tmp_string_505+3));
char *__tmp_string_504="an";
 __boundcheck_metadata_store((void *)(__tmp_string_504),(void *)(__tmp_string_504+2));
char *__tmp_string_503="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_503),(void *)(__tmp_string_503+3));
char *__tmp_string_502="you";
 __boundcheck_metadata_store((void *)(__tmp_string_502),(void *)(__tmp_string_502+3));
char *__tmp_string_501="20";
 __boundcheck_metadata_store((void *)(__tmp_string_501),(void *)(__tmp_string_501+2));
char *__tmp_string_500="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_500),(void *)(__tmp_string_500+5));
char *__tmp_string_499="and";
 __boundcheck_metadata_store((void *)(__tmp_string_499),(void *)(__tmp_string_499+3));
char *__tmp_string_498="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_498),(void *)(__tmp_string_498+2));
char *__tmp_string_497="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_497),(void *)(__tmp_string_497+5));
char *__tmp_string_496="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_496),(void *)(__tmp_string_496+3));
char *__tmp_string_495="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_495),(void *)(__tmp_string_495+5));
char *__tmp_string_494="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_494),(void *)(__tmp_string_494+6));
char *__tmp_string_493="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_493),(void *)(__tmp_string_493+6));
char *__tmp_string_492="where";
 __boundcheck_metadata_store((void *)(__tmp_string_492),(void *)(__tmp_string_492+5));
char *__tmp_string_491="have";
 __boundcheck_metadata_store((void *)(__tmp_string_491),(void *)(__tmp_string_491+4));
char *__tmp_string_490="long";
 __boundcheck_metadata_store((void *)(__tmp_string_490),(void *)(__tmp_string_490+4));
char *__tmp_string_489="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_489),(void *)(__tmp_string_489+3));
char *__tmp_string_488="for";
 __boundcheck_metadata_store((void *)(__tmp_string_488),(void *)(__tmp_string_488+3));
char *__tmp_string_487="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_487),(void *)(__tmp_string_487+4));
char *__tmp_string_486="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_486),(void *)(__tmp_string_486+4));
char *__tmp_string_485="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_485),(void *)(__tmp_string_485+4));
char *__tmp_string_484="me";
 __boundcheck_metadata_store((void *)(__tmp_string_484),(void *)(__tmp_string_484+2));
char *__tmp_string_483="more";
 __boundcheck_metadata_store((void *)(__tmp_string_483),(void *)(__tmp_string_483+4));
char *__tmp_string_482="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_482),(void *)(__tmp_string_482+5));
char *__tmp_string_481="the";
 __boundcheck_metadata_store((void *)(__tmp_string_481),(void *)(__tmp_string_481+3));
char *__tmp_string_480="past";
 __boundcheck_metadata_store((void *)(__tmp_string_480),(void *)(__tmp_string_480+4));
char *__tmp_string_479="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_479),(void *)(__tmp_string_479+10));
char *__tmp_string_478="with";
 __boundcheck_metadata_store((void *)(__tmp_string_478),(void *)(__tmp_string_478+4));
char *__tmp_string_477="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_477),(void *)(__tmp_string_477+7));
char *__tmp_string_476="time";
 __boundcheck_metadata_store((void *)(__tmp_string_476),(void *)(__tmp_string_476+4));
char *__tmp_string_475="too";
 __boundcheck_metadata_store((void *)(__tmp_string_475),(void *)(__tmp_string_475+3));
char *__tmp_string_474="one";
 __boundcheck_metadata_store((void *)(__tmp_string_474),(void *)(__tmp_string_474+3));
char *__tmp_string_473="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_473),(void *)(__tmp_string_473+6));
char *__tmp_string_472="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_472),(void *)(__tmp_string_472+5));
char *__tmp_string_471="support";
 __boundcheck_metadata_store((void *)(__tmp_string_471),(void *)(__tmp_string_471+7));
char *__tmp_string_470="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_470),(void *)(__tmp_string_470+4));
char *__tmp_string_469="their";
 __boundcheck_metadata_store((void *)(__tmp_string_469),(void *)(__tmp_string_469+5));
char *__tmp_string_468="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_468),(void *)(__tmp_string_468+10));
char *__tmp_string_467="size";
 __boundcheck_metadata_store((void *)(__tmp_string_467),(void *)(__tmp_string_467+4));
char *__tmp_string_466="get";
 __boundcheck_metadata_store((void *)(__tmp_string_466),(void *)(__tmp_string_466+3));
char *__tmp_string_465="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_465),(void *)(__tmp_string_465+11));
char *__tmp_string_464="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_464),(void *)(__tmp_string_464+6));
char *__tmp_string_463="before";
 __boundcheck_metadata_store((void *)(__tmp_string_463),(void *)(__tmp_string_463+6));
char *__tmp_string_462="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_462),(void *)(__tmp_string_462+8));
char *__tmp_string_461="it";
 __boundcheck_metadata_store((void *)(__tmp_string_461),(void *)(__tmp_string_461+2));
char *__tmp_string_460="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_460),(void *)(__tmp_string_460+3));
char *__tmp_string_459="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_459),(void *)(__tmp_string_459+3));
char *__tmp_string_458="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_458),(void *)(__tmp_string_458+3));
char *__tmp_string_457="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_457),(void *)(__tmp_string_457+3));
char *__tmp_string_456="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_456),(void *)(__tmp_string_456+3));
char *__tmp_string_455="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_455),(void *)(__tmp_string_455+3));
char *__tmp_string_454="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_454),(void *)(__tmp_string_454+3));
char *__tmp_string_453="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_453),(void *)(__tmp_string_453+3));
char *__tmp_string_452="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_452),(void *)(__tmp_string_452+3));
char *__tmp_string_451="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_451),(void *)(__tmp_string_451+3));
char *__tmp_string_450="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_450),(void *)(__tmp_string_450+3));
char *__tmp_string_449="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_449),(void *)(__tmp_string_449+3));
char *__tmp_string_448="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_448),(void *)(__tmp_string_448+3));
char *__tmp_string_447="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_447),(void *)(__tmp_string_447+3));
char *__tmp_string_446="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_446),(void *)(__tmp_string_446+3));
char *__tmp_string_445="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_445),(void *)(__tmp_string_445+4));
char *__tmp_string_444="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_444),(void *)(__tmp_string_444+6));
char *__tmp_string_443="you";
 __boundcheck_metadata_store((void *)(__tmp_string_443),(void *)(__tmp_string_443+3));
char *__tmp_string_442="room";
 __boundcheck_metadata_store((void *)(__tmp_string_442),(void *)(__tmp_string_442+4));
char *__tmp_string_441="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_441),(void *)(__tmp_string_441+7));
char *__tmp_string_440="own";
 __boundcheck_metadata_store((void *)(__tmp_string_440),(void *)(__tmp_string_440+3));
char *__tmp_string_439="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_439),(void *)(__tmp_string_439+8));
char *__tmp_string_438="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_438),(void *)(__tmp_string_438+3));
char *__tmp_string_437="it";
 __boundcheck_metadata_store((void *)(__tmp_string_437),(void *)(__tmp_string_437+2));
char *__tmp_string_436="other";
 __boundcheck_metadata_store((void *)(__tmp_string_436),(void *)(__tmp_string_436+5));
char *__tmp_string_435="be";
 __boundcheck_metadata_store((void *)(__tmp_string_435),(void *)(__tmp_string_435+2));
char *__tmp_string_434="it";
 __boundcheck_metadata_store((void *)(__tmp_string_434),(void *)(__tmp_string_434+2));
char *__tmp_string_433="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_433),(void *)(__tmp_string_433+5));
char *__tmp_string_432="are";
 __boundcheck_metadata_store((void *)(__tmp_string_432),(void *)(__tmp_string_432+3));
char *__tmp_string_431="either";
 __boundcheck_metadata_store((void *)(__tmp_string_431),(void *)(__tmp_string_431+6));
char *__tmp_string_430="much";
 __boundcheck_metadata_store((void *)(__tmp_string_430),(void *)(__tmp_string_430+4));
char *__tmp_string_429="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_429),(void *)(__tmp_string_429+12));
char *__tmp_string_428="your";
 __boundcheck_metadata_store((void *)(__tmp_string_428),(void *)(__tmp_string_428+4));
char *__tmp_string_427="the";
 __boundcheck_metadata_store((void *)(__tmp_string_427),(void *)(__tmp_string_427+3));
char *__tmp_string_426="you";
 __boundcheck_metadata_store((void *)(__tmp_string_426),(void *)(__tmp_string_426+3));
char *__tmp_string_425="may";
 __boundcheck_metadata_store((void *)(__tmp_string_425),(void *)(__tmp_string_425+3));
char *__tmp_string_424="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_424),(void *)(__tmp_string_424+5));
char *__tmp_string_423="them";
 __boundcheck_metadata_store((void *)(__tmp_string_423),(void *)(__tmp_string_423+4));
char *__tmp_string_422="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_422),(void *)(__tmp_string_422+3));
char *__tmp_string_421="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_421),(void *)(__tmp_string_421+3));
char *__tmp_string_420="40";
 __boundcheck_metadata_store((void *)(__tmp_string_420),(void *)(__tmp_string_420+2));
char *__tmp_string_419="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_419),(void *)(__tmp_string_419+3));
char *__tmp_string_418="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_418),(void *)(__tmp_string_418+3));
char *__tmp_string_417="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_417),(void *)(__tmp_string_417+3));
char *__tmp_string_416="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_416),(void *)(__tmp_string_416+3));
char *__tmp_string_415="life";
 __boundcheck_metadata_store((void *)(__tmp_string_415),(void *)(__tmp_string_415+4));
char *__tmp_string_414="what";
 __boundcheck_metadata_store((void *)(__tmp_string_414),(void *)(__tmp_string_414+4));
char *__tmp_string_413="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_413),(void *)(__tmp_string_413+6));
char *__tmp_string_412="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_412),(void *)(__tmp_string_412+2));
char *__tmp_string_411="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_411),(void *)(__tmp_string_411+3));
char *__tmp_string_410="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_410),(void *)(__tmp_string_410+3));
char *__tmp_string_409="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_409),(void *)(__tmp_string_409+7));
char *__tmp_string_408="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_408),(void *)(__tmp_string_408+6));
char *__tmp_string_407="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_407),(void *)(__tmp_string_407+8));
char *__tmp_string_406="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_406),(void *)(__tmp_string_406+3));
char *__tmp_string_405="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_405),(void *)(__tmp_string_405+2));
char *__tmp_string_404="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_404),(void *)(__tmp_string_404+3));
char *__tmp_string_403="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_403),(void *)(__tmp_string_403+3));
char *__tmp_string_402="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_402),(void *)(__tmp_string_402+3));
char *__tmp_string_401="people";
 __boundcheck_metadata_store((void *)(__tmp_string_401),(void *)(__tmp_string_401+6));
char *__tmp_string_400="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_400),(void *)(__tmp_string_400+5));
char *__tmp_string_399="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_399),(void *)(__tmp_string_399+2));
char *__tmp_string_398="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_398),(void *)(__tmp_string_398+4));
char *__tmp_string_397="4";
 __boundcheck_metadata_store((void *)(__tmp_string_397),(void *)(__tmp_string_397+1));
char *__tmp_string_396="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_396),(void *)(__tmp_string_396+4));
char *__tmp_string_395="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_395),(void *)(__tmp_string_395+2));
char *__tmp_string_394="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_394),(void *)(__tmp_string_394+3));
char *__tmp_string_393="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_393),(void *)(__tmp_string_393+2));
char *__tmp_string_392="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_392),(void *)(__tmp_string_392+3));
char *__tmp_string_391="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_391),(void *)(__tmp_string_391+3));
char *__tmp_string_390="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_390),(void *)(__tmp_string_390+3));
char *__tmp_string_389="th";
 __boundcheck_metadata_store((void *)(__tmp_string_389),(void *)(__tmp_string_389+2));
char *__tmp_string_388="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_388),(void *)(__tmp_string_388+4));
char *__tmp_string_387="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_387),(void *)(__tmp_string_387+3));
char *__tmp_string_386="an";
 __boundcheck_metadata_store((void *)(__tmp_string_386),(void *)(__tmp_string_386+2));
char *__tmp_string_385="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_385),(void *)(__tmp_string_385+9));
char *__tmp_string_384="way";
 __boundcheck_metadata_store((void *)(__tmp_string_384),(void *)(__tmp_string_384+3));
char *__tmp_string_383="an";
 __boundcheck_metadata_store((void *)(__tmp_string_383),(void *)(__tmp_string_383+2));
char *__tmp_string_382="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_382),(void *)(__tmp_string_382+3));
char *__tmp_string_381="you";
 __boundcheck_metadata_store((void *)(__tmp_string_381),(void *)(__tmp_string_381+3));
char *__tmp_string_380="20";
 __boundcheck_metadata_store((void *)(__tmp_string_380),(void *)(__tmp_string_380+2));
char *__tmp_string_379="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_379),(void *)(__tmp_string_379+5));
char *__tmp_string_378="and";
 __boundcheck_metadata_store((void *)(__tmp_string_378),(void *)(__tmp_string_378+3));
char *__tmp_string_377="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_377),(void *)(__tmp_string_377+2));
char *__tmp_string_376="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_376),(void *)(__tmp_string_376+5));
char *__tmp_string_375="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_375),(void *)(__tmp_string_375+3));
char *__tmp_string_374="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_374),(void *)(__tmp_string_374+5));
char *__tmp_string_373="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_373),(void *)(__tmp_string_373+6));
char *__tmp_string_372="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_372),(void *)(__tmp_string_372+6));
char *__tmp_string_371="where";
 __boundcheck_metadata_store((void *)(__tmp_string_371),(void *)(__tmp_string_371+5));
char *__tmp_string_370="have";
 __boundcheck_metadata_store((void *)(__tmp_string_370),(void *)(__tmp_string_370+4));
char *__tmp_string_369="long";
 __boundcheck_metadata_store((void *)(__tmp_string_369),(void *)(__tmp_string_369+4));
char *__tmp_string_368="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_368),(void *)(__tmp_string_368+3));
char *__tmp_string_367="for";
 __boundcheck_metadata_store((void *)(__tmp_string_367),(void *)(__tmp_string_367+3));
char *__tmp_string_366="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_366),(void *)(__tmp_string_366+4));
char *__tmp_string_365="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_365),(void *)(__tmp_string_365+4));
char *__tmp_string_364="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_364),(void *)(__tmp_string_364+4));
char *__tmp_string_363="me";
 __boundcheck_metadata_store((void *)(__tmp_string_363),(void *)(__tmp_string_363+2));
char *__tmp_string_362="more";
 __boundcheck_metadata_store((void *)(__tmp_string_362),(void *)(__tmp_string_362+4));
char *__tmp_string_361="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_361),(void *)(__tmp_string_361+5));
char *__tmp_string_360="the";
 __boundcheck_metadata_store((void *)(__tmp_string_360),(void *)(__tmp_string_360+3));
char *__tmp_string_359="past";
 __boundcheck_metadata_store((void *)(__tmp_string_359),(void *)(__tmp_string_359+4));
char *__tmp_string_358="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_358),(void *)(__tmp_string_358+10));
char *__tmp_string_357="with";
 __boundcheck_metadata_store((void *)(__tmp_string_357),(void *)(__tmp_string_357+4));
char *__tmp_string_356="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_356),(void *)(__tmp_string_356+7));
char *__tmp_string_355="time";
 __boundcheck_metadata_store((void *)(__tmp_string_355),(void *)(__tmp_string_355+4));
char *__tmp_string_354="too";
 __boundcheck_metadata_store((void *)(__tmp_string_354),(void *)(__tmp_string_354+3));
char *__tmp_string_353="one";
 __boundcheck_metadata_store((void *)(__tmp_string_353),(void *)(__tmp_string_353+3));
char *__tmp_string_352="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_352),(void *)(__tmp_string_352+6));
char *__tmp_string_351="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_351),(void *)(__tmp_string_351+5));
char *__tmp_string_350="support";
 __boundcheck_metadata_store((void *)(__tmp_string_350),(void *)(__tmp_string_350+7));
char *__tmp_string_349="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_349),(void *)(__tmp_string_349+4));
char *__tmp_string_348="their";
 __boundcheck_metadata_store((void *)(__tmp_string_348),(void *)(__tmp_string_348+5));
char *__tmp_string_347="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_347),(void *)(__tmp_string_347+10));
char *__tmp_string_346="size";
 __boundcheck_metadata_store((void *)(__tmp_string_346),(void *)(__tmp_string_346+4));
char *__tmp_string_345="get";
 __boundcheck_metadata_store((void *)(__tmp_string_345),(void *)(__tmp_string_345+3));
char *__tmp_string_344="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_344),(void *)(__tmp_string_344+11));
char *__tmp_string_343="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_343),(void *)(__tmp_string_343+6));
char *__tmp_string_342="before";
 __boundcheck_metadata_store((void *)(__tmp_string_342),(void *)(__tmp_string_342+6));
char *__tmp_string_341="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_341),(void *)(__tmp_string_341+8));
char *__tmp_string_340="it";
 __boundcheck_metadata_store((void *)(__tmp_string_340),(void *)(__tmp_string_340+2));
char *__tmp_string_339="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_339),(void *)(__tmp_string_339+3));
char *__tmp_string_338="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_338),(void *)(__tmp_string_338+3));
char *__tmp_string_337="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_337),(void *)(__tmp_string_337+3));
char *__tmp_string_336="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_336),(void *)(__tmp_string_336+3));
char *__tmp_string_335="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_335),(void *)(__tmp_string_335+3));
char *__tmp_string_334="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_334),(void *)(__tmp_string_334+3));
char *__tmp_string_333="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_333),(void *)(__tmp_string_333+3));
char *__tmp_string_332="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_332),(void *)(__tmp_string_332+3));
char *__tmp_string_331="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_331),(void *)(__tmp_string_331+3));
char *__tmp_string_330="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_330),(void *)(__tmp_string_330+3));
char *__tmp_string_329="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_329),(void *)(__tmp_string_329+3));
char *__tmp_string_328="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_328),(void *)(__tmp_string_328+3));
char *__tmp_string_327="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_327),(void *)(__tmp_string_327+3));
char *__tmp_string_326="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_326),(void *)(__tmp_string_326+3));
char *__tmp_string_325="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_325),(void *)(__tmp_string_325+3));
char *__tmp_string_324="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_324),(void *)(__tmp_string_324+4));
char *__tmp_string_323="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_323),(void *)(__tmp_string_323+6));
char *__tmp_string_322="you";
 __boundcheck_metadata_store((void *)(__tmp_string_322),(void *)(__tmp_string_322+3));
char *__tmp_string_321="room";
 __boundcheck_metadata_store((void *)(__tmp_string_321),(void *)(__tmp_string_321+4));
char *__tmp_string_320="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_320),(void *)(__tmp_string_320+7));
char *__tmp_string_319="own";
 __boundcheck_metadata_store((void *)(__tmp_string_319),(void *)(__tmp_string_319+3));
char *__tmp_string_318="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_318),(void *)(__tmp_string_318+8));
char *__tmp_string_317="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_317),(void *)(__tmp_string_317+3));
char *__tmp_string_316="it";
 __boundcheck_metadata_store((void *)(__tmp_string_316),(void *)(__tmp_string_316+2));
char *__tmp_string_315="other";
 __boundcheck_metadata_store((void *)(__tmp_string_315),(void *)(__tmp_string_315+5));
char *__tmp_string_314="be";
 __boundcheck_metadata_store((void *)(__tmp_string_314),(void *)(__tmp_string_314+2));
char *__tmp_string_313="it";
 __boundcheck_metadata_store((void *)(__tmp_string_313),(void *)(__tmp_string_313+2));
char *__tmp_string_312="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_312),(void *)(__tmp_string_312+5));
char *__tmp_string_311="are";
 __boundcheck_metadata_store((void *)(__tmp_string_311),(void *)(__tmp_string_311+3));
char *__tmp_string_310="either";
 __boundcheck_metadata_store((void *)(__tmp_string_310),(void *)(__tmp_string_310+6));
char *__tmp_string_309="much";
 __boundcheck_metadata_store((void *)(__tmp_string_309),(void *)(__tmp_string_309+4));
char *__tmp_string_308="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_308),(void *)(__tmp_string_308+12));
char *__tmp_string_307="your";
 __boundcheck_metadata_store((void *)(__tmp_string_307),(void *)(__tmp_string_307+4));
char *__tmp_string_306="the";
 __boundcheck_metadata_store((void *)(__tmp_string_306),(void *)(__tmp_string_306+3));
char *__tmp_string_305="you";
 __boundcheck_metadata_store((void *)(__tmp_string_305),(void *)(__tmp_string_305+3));
char *__tmp_string_304="may";
 __boundcheck_metadata_store((void *)(__tmp_string_304),(void *)(__tmp_string_304+3));
char *__tmp_string_303="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_303),(void *)(__tmp_string_303+5));
char *__tmp_string_302="them";
 __boundcheck_metadata_store((void *)(__tmp_string_302),(void *)(__tmp_string_302+4));
char *__tmp_string_301="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_301),(void *)(__tmp_string_301+3));
char *__tmp_string_300="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_300),(void *)(__tmp_string_300+3));
char *__tmp_string_299="40";
 __boundcheck_metadata_store((void *)(__tmp_string_299),(void *)(__tmp_string_299+2));
char *__tmp_string_298="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_298),(void *)(__tmp_string_298+3));
char *__tmp_string_297="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_297),(void *)(__tmp_string_297+3));
char *__tmp_string_296="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_296),(void *)(__tmp_string_296+3));
char *__tmp_string_295="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_295),(void *)(__tmp_string_295+3));
char *__tmp_string_294="life";
 __boundcheck_metadata_store((void *)(__tmp_string_294),(void *)(__tmp_string_294+4));
char *__tmp_string_293="what";
 __boundcheck_metadata_store((void *)(__tmp_string_293),(void *)(__tmp_string_293+4));
char *__tmp_string_292="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_292),(void *)(__tmp_string_292+6));
char *__tmp_string_291="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_291),(void *)(__tmp_string_291+2));
char *__tmp_string_290="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_290),(void *)(__tmp_string_290+3));
char *__tmp_string_289="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_289),(void *)(__tmp_string_289+3));
char *__tmp_string_288="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_288),(void *)(__tmp_string_288+7));
char *__tmp_string_287="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_287),(void *)(__tmp_string_287+6));
char *__tmp_string_286="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_286),(void *)(__tmp_string_286+8));
char *__tmp_string_285="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_285),(void *)(__tmp_string_285+3));
char *__tmp_string_284="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_284),(void *)(__tmp_string_284+2));
char *__tmp_string_283="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_283),(void *)(__tmp_string_283+3));
char *__tmp_string_282="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_282),(void *)(__tmp_string_282+3));
char *__tmp_string_281="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_281),(void *)(__tmp_string_281+3));
char *__tmp_string_280="people";
 __boundcheck_metadata_store((void *)(__tmp_string_280),(void *)(__tmp_string_280+6));
char *__tmp_string_279="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_279),(void *)(__tmp_string_279+5));
char *__tmp_string_278="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_278),(void *)(__tmp_string_278+2));
char *__tmp_string_277="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_277),(void *)(__tmp_string_277+4));
char *__tmp_string_276="4";
 __boundcheck_metadata_store((void *)(__tmp_string_276),(void *)(__tmp_string_276+1));
char *__tmp_string_275="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_275),(void *)(__tmp_string_275+4));
char *__tmp_string_274="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_274),(void *)(__tmp_string_274+2));
char *__tmp_string_273="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_273),(void *)(__tmp_string_273+3));
char *__tmp_string_272="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_272),(void *)(__tmp_string_272+2));
char *__tmp_string_271="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_271),(void *)(__tmp_string_271+3));
char *__tmp_string_270="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_270),(void *)(__tmp_string_270+3));
char *__tmp_string_269="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_269),(void *)(__tmp_string_269+3));
char *__tmp_string_268="th";
 __boundcheck_metadata_store((void *)(__tmp_string_268),(void *)(__tmp_string_268+2));
char *__tmp_string_267="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_267),(void *)(__tmp_string_267+4));
char *__tmp_string_266="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_266),(void *)(__tmp_string_266+3));
char *__tmp_string_265="an";
 __boundcheck_metadata_store((void *)(__tmp_string_265),(void *)(__tmp_string_265+2));
char *__tmp_string_264="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_264),(void *)(__tmp_string_264+9));
char *__tmp_string_263="way";
 __boundcheck_metadata_store((void *)(__tmp_string_263),(void *)(__tmp_string_263+3));
char *__tmp_string_262="an";
 __boundcheck_metadata_store((void *)(__tmp_string_262),(void *)(__tmp_string_262+2));
char *__tmp_string_261="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_261),(void *)(__tmp_string_261+3));
char *__tmp_string_260="you";
 __boundcheck_metadata_store((void *)(__tmp_string_260),(void *)(__tmp_string_260+3));
char *__tmp_string_259="20";
 __boundcheck_metadata_store((void *)(__tmp_string_259),(void *)(__tmp_string_259+2));
char *__tmp_string_258="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_258),(void *)(__tmp_string_258+5));
char *__tmp_string_257="and";
 __boundcheck_metadata_store((void *)(__tmp_string_257),(void *)(__tmp_string_257+3));
char *__tmp_string_256="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_256),(void *)(__tmp_string_256+2));
char *__tmp_string_255="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_255),(void *)(__tmp_string_255+5));
char *__tmp_string_254="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_254),(void *)(__tmp_string_254+3));
char *__tmp_string_253="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_253),(void *)(__tmp_string_253+5));
char *__tmp_string_252="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_252),(void *)(__tmp_string_252+6));
char *__tmp_string_251="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_251),(void *)(__tmp_string_251+6));
char *__tmp_string_250="where";
 __boundcheck_metadata_store((void *)(__tmp_string_250),(void *)(__tmp_string_250+5));
char *__tmp_string_249="have";
 __boundcheck_metadata_store((void *)(__tmp_string_249),(void *)(__tmp_string_249+4));
char *__tmp_string_248="long";
 __boundcheck_metadata_store((void *)(__tmp_string_248),(void *)(__tmp_string_248+4));
char *__tmp_string_247="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_247),(void *)(__tmp_string_247+3));
char *__tmp_string_246="for";
 __boundcheck_metadata_store((void *)(__tmp_string_246),(void *)(__tmp_string_246+3));
char *__tmp_string_245="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_245),(void *)(__tmp_string_245+4));
char *__tmp_string_244="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_244),(void *)(__tmp_string_244+4));
char *__tmp_string_243="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_243),(void *)(__tmp_string_243+4));
char *__tmp_string_242="me";
 __boundcheck_metadata_store((void *)(__tmp_string_242),(void *)(__tmp_string_242+2));
char *__tmp_string_241="more";
 __boundcheck_metadata_store((void *)(__tmp_string_241),(void *)(__tmp_string_241+4));
char *__tmp_string_240="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_240),(void *)(__tmp_string_240+5));
char *__tmp_string_239="the";
 __boundcheck_metadata_store((void *)(__tmp_string_239),(void *)(__tmp_string_239+3));
char *__tmp_string_238="past";
 __boundcheck_metadata_store((void *)(__tmp_string_238),(void *)(__tmp_string_238+4));
char *__tmp_string_237="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_237),(void *)(__tmp_string_237+10));
char *__tmp_string_236="with";
 __boundcheck_metadata_store((void *)(__tmp_string_236),(void *)(__tmp_string_236+4));
char *__tmp_string_235="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_235),(void *)(__tmp_string_235+7));
char *__tmp_string_234="time";
 __boundcheck_metadata_store((void *)(__tmp_string_234),(void *)(__tmp_string_234+4));
char *__tmp_string_233="too";
 __boundcheck_metadata_store((void *)(__tmp_string_233),(void *)(__tmp_string_233+3));
char *__tmp_string_232="one";
 __boundcheck_metadata_store((void *)(__tmp_string_232),(void *)(__tmp_string_232+3));
char *__tmp_string_231="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_231),(void *)(__tmp_string_231+6));
char *__tmp_string_230="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_230),(void *)(__tmp_string_230+5));
char *__tmp_string_229="support";
 __boundcheck_metadata_store((void *)(__tmp_string_229),(void *)(__tmp_string_229+7));
char *__tmp_string_228="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_228),(void *)(__tmp_string_228+4));
char *__tmp_string_227="their";
 __boundcheck_metadata_store((void *)(__tmp_string_227),(void *)(__tmp_string_227+5));
char *__tmp_string_226="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_226),(void *)(__tmp_string_226+10));
char *__tmp_string_225="size";
 __boundcheck_metadata_store((void *)(__tmp_string_225),(void *)(__tmp_string_225+4));
char *__tmp_string_224="get";
 __boundcheck_metadata_store((void *)(__tmp_string_224),(void *)(__tmp_string_224+3));
char *__tmp_string_223="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_223),(void *)(__tmp_string_223+11));
char *__tmp_string_222="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_222),(void *)(__tmp_string_222+6));
char *__tmp_string_221="before";
 __boundcheck_metadata_store((void *)(__tmp_string_221),(void *)(__tmp_string_221+6));
char *__tmp_string_220="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_220),(void *)(__tmp_string_220+8));
char *__tmp_string_219="it";
 __boundcheck_metadata_store((void *)(__tmp_string_219),(void *)(__tmp_string_219+2));
char *__tmp_string_218="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_218),(void *)(__tmp_string_218+3));
char *__tmp_string_217="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_217),(void *)(__tmp_string_217+3));
char *__tmp_string_216="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_216),(void *)(__tmp_string_216+3));
char *__tmp_string_215="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_215),(void *)(__tmp_string_215+3));
char *__tmp_string_214="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_214),(void *)(__tmp_string_214+3));
char *__tmp_string_213="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_213),(void *)(__tmp_string_213+3));
char *__tmp_string_212="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_212),(void *)(__tmp_string_212+3));
char *__tmp_string_211="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_211),(void *)(__tmp_string_211+3));
char *__tmp_string_210="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_210),(void *)(__tmp_string_210+3));
char *__tmp_string_209="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_209),(void *)(__tmp_string_209+3));
char *__tmp_string_208="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_208),(void *)(__tmp_string_208+3));
char *__tmp_string_207="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_207),(void *)(__tmp_string_207+3));
char *__tmp_string_206="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_206),(void *)(__tmp_string_206+3));
char *__tmp_string_205="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_205),(void *)(__tmp_string_205+3));
char *__tmp_string_204="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_204),(void *)(__tmp_string_204+3));
char *__tmp_string_203="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_203),(void *)(__tmp_string_203+4));
char *__tmp_string_202="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_202),(void *)(__tmp_string_202+6));
char *__tmp_string_201="you";
 __boundcheck_metadata_store((void *)(__tmp_string_201),(void *)(__tmp_string_201+3));
char *__tmp_string_200="room";
 __boundcheck_metadata_store((void *)(__tmp_string_200),(void *)(__tmp_string_200+4));
char *__tmp_string_199="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_199),(void *)(__tmp_string_199+7));
char *__tmp_string_198="own";
 __boundcheck_metadata_store((void *)(__tmp_string_198),(void *)(__tmp_string_198+3));
char *__tmp_string_197="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_197),(void *)(__tmp_string_197+8));
char *__tmp_string_196="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_196),(void *)(__tmp_string_196+3));
char *__tmp_string_195="it";
 __boundcheck_metadata_store((void *)(__tmp_string_195),(void *)(__tmp_string_195+2));
char *__tmp_string_194="other";
 __boundcheck_metadata_store((void *)(__tmp_string_194),(void *)(__tmp_string_194+5));
char *__tmp_string_193="be";
 __boundcheck_metadata_store((void *)(__tmp_string_193),(void *)(__tmp_string_193+2));
char *__tmp_string_192="it";
 __boundcheck_metadata_store((void *)(__tmp_string_192),(void *)(__tmp_string_192+2));
char *__tmp_string_191="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_191),(void *)(__tmp_string_191+5));
char *__tmp_string_190="are";
 __boundcheck_metadata_store((void *)(__tmp_string_190),(void *)(__tmp_string_190+3));
char *__tmp_string_189="either";
 __boundcheck_metadata_store((void *)(__tmp_string_189),(void *)(__tmp_string_189+6));
char *__tmp_string_188="much";
 __boundcheck_metadata_store((void *)(__tmp_string_188),(void *)(__tmp_string_188+4));
char *__tmp_string_187="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_187),(void *)(__tmp_string_187+12));
char *__tmp_string_186="your";
 __boundcheck_metadata_store((void *)(__tmp_string_186),(void *)(__tmp_string_186+4));
char *__tmp_string_185="the";
 __boundcheck_metadata_store((void *)(__tmp_string_185),(void *)(__tmp_string_185+3));
char *__tmp_string_184="you";
 __boundcheck_metadata_store((void *)(__tmp_string_184),(void *)(__tmp_string_184+3));
char *__tmp_string_183="may";
 __boundcheck_metadata_store((void *)(__tmp_string_183),(void *)(__tmp_string_183+3));
char *__tmp_string_182="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_182),(void *)(__tmp_string_182+5));
char *__tmp_string_181="them";
 __boundcheck_metadata_store((void *)(__tmp_string_181),(void *)(__tmp_string_181+4));
char *__tmp_string_180="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_180),(void *)(__tmp_string_180+3));
char *__tmp_string_179="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_179),(void *)(__tmp_string_179+3));
char *__tmp_string_178="40";
 __boundcheck_metadata_store((void *)(__tmp_string_178),(void *)(__tmp_string_178+2));
char *__tmp_string_177="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_177),(void *)(__tmp_string_177+3));
char *__tmp_string_176="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_176),(void *)(__tmp_string_176+3));
char *__tmp_string_175="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_175),(void *)(__tmp_string_175+3));
char *__tmp_string_174="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_174),(void *)(__tmp_string_174+3));
char *__tmp_string_173="life";
 __boundcheck_metadata_store((void *)(__tmp_string_173),(void *)(__tmp_string_173+4));
char *__tmp_string_172="what";
 __boundcheck_metadata_store((void *)(__tmp_string_172),(void *)(__tmp_string_172+4));
char *__tmp_string_171="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_171),(void *)(__tmp_string_171+6));
char *__tmp_string_170="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_170),(void *)(__tmp_string_170+2));
char *__tmp_string_169="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_169),(void *)(__tmp_string_169+3));
char *__tmp_string_168="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_168),(void *)(__tmp_string_168+3));
char *__tmp_string_167="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_167),(void *)(__tmp_string_167+7));
char *__tmp_string_166="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_166),(void *)(__tmp_string_166+6));
char *__tmp_string_165="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_165),(void *)(__tmp_string_165+8));
char *__tmp_string_164="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_164),(void *)(__tmp_string_164+3));
char *__tmp_string_163="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_163),(void *)(__tmp_string_163+2));
char *__tmp_string_162="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_162),(void *)(__tmp_string_162+3));
char *__tmp_string_161="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_161),(void *)(__tmp_string_161+3));
char *__tmp_string_160="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_160),(void *)(__tmp_string_160+3));
char *__tmp_string_159="people";
 __boundcheck_metadata_store((void *)(__tmp_string_159),(void *)(__tmp_string_159+6));
char *__tmp_string_158="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_158),(void *)(__tmp_string_158+5));
char *__tmp_string_157="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_157),(void *)(__tmp_string_157+2));
char *__tmp_string_156="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_156),(void *)(__tmp_string_156+4));
char *__tmp_string_155="4";
 __boundcheck_metadata_store((void *)(__tmp_string_155),(void *)(__tmp_string_155+1));
char *__tmp_string_154="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_154),(void *)(__tmp_string_154+4));
char *__tmp_string_153="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_153),(void *)(__tmp_string_153+2));
char *__tmp_string_152="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_152),(void *)(__tmp_string_152+3));
char *__tmp_string_151="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_151),(void *)(__tmp_string_151+2));
char *__tmp_string_150="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_150),(void *)(__tmp_string_150+3));
char *__tmp_string_149="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_149),(void *)(__tmp_string_149+3));
char *__tmp_string_148="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_148),(void *)(__tmp_string_148+3));
char *__tmp_string_147="th";
 __boundcheck_metadata_store((void *)(__tmp_string_147),(void *)(__tmp_string_147+2));
char *__tmp_string_146="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_146),(void *)(__tmp_string_146+4));
char *__tmp_string_145="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_145),(void *)(__tmp_string_145+3));
char *__tmp_string_144="an";
 __boundcheck_metadata_store((void *)(__tmp_string_144),(void *)(__tmp_string_144+2));
char *__tmp_string_143="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_143),(void *)(__tmp_string_143+9));
char *__tmp_string_142="way";
 __boundcheck_metadata_store((void *)(__tmp_string_142),(void *)(__tmp_string_142+3));
char *__tmp_string_141="an";
 __boundcheck_metadata_store((void *)(__tmp_string_141),(void *)(__tmp_string_141+2));
char *__tmp_string_140="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_140),(void *)(__tmp_string_140+3));
char *__tmp_string_139="you";
 __boundcheck_metadata_store((void *)(__tmp_string_139),(void *)(__tmp_string_139+3));
char *__tmp_string_138="20";
 __boundcheck_metadata_store((void *)(__tmp_string_138),(void *)(__tmp_string_138+2));
char *__tmp_string_137="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_137),(void *)(__tmp_string_137+5));
char *__tmp_string_136="and";
 __boundcheck_metadata_store((void *)(__tmp_string_136),(void *)(__tmp_string_136+3));
char *__tmp_string_135="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_135),(void *)(__tmp_string_135+2));
char *__tmp_string_134="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_134),(void *)(__tmp_string_134+5));
char *__tmp_string_133="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_133),(void *)(__tmp_string_133+3));
char *__tmp_string_132="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_132),(void *)(__tmp_string_132+5));
char *__tmp_string_131="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_131),(void *)(__tmp_string_131+6));
char *__tmp_string_130="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_130),(void *)(__tmp_string_130+6));
char *__tmp_string_129="where";
 __boundcheck_metadata_store((void *)(__tmp_string_129),(void *)(__tmp_string_129+5));
char *__tmp_string_128="have";
 __boundcheck_metadata_store((void *)(__tmp_string_128),(void *)(__tmp_string_128+4));
char *__tmp_string_127="long";
 __boundcheck_metadata_store((void *)(__tmp_string_127),(void *)(__tmp_string_127+4));
char *__tmp_string_126="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_126),(void *)(__tmp_string_126+3));
char *__tmp_string_125="for";
 __boundcheck_metadata_store((void *)(__tmp_string_125),(void *)(__tmp_string_125+3));
char *__tmp_string_124="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_124),(void *)(__tmp_string_124+4));
char *__tmp_string_123="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_123),(void *)(__tmp_string_123+4));
char *__tmp_string_122="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_122),(void *)(__tmp_string_122+4));
char *__tmp_string_121="me";
 __boundcheck_metadata_store((void *)(__tmp_string_121),(void *)(__tmp_string_121+2));
char *__tmp_string_120="more";
 __boundcheck_metadata_store((void *)(__tmp_string_120),(void *)(__tmp_string_120+4));
char *__tmp_string_119="parts";
 __boundcheck_metadata_store((void *)(__tmp_string_119),(void *)(__tmp_string_119+5));
char *__tmp_string_118="the";
 __boundcheck_metadata_store((void *)(__tmp_string_118),(void *)(__tmp_string_118+3));
char *__tmp_string_117="past";
 __boundcheck_metadata_store((void *)(__tmp_string_117),(void *)(__tmp_string_117+4));
char *__tmp_string_116="Dispensing";
 __boundcheck_metadata_store((void *)(__tmp_string_116),(void *)(__tmp_string_116+10));
char *__tmp_string_115="with";
 __boundcheck_metadata_store((void *)(__tmp_string_115),(void *)(__tmp_string_115+4));
char *__tmp_string_114="careful";
 __boundcheck_metadata_store((void *)(__tmp_string_114),(void *)(__tmp_string_114+7));
char *__tmp_string_113="time";
 __boundcheck_metadata_store((void *)(__tmp_string_113),(void *)(__tmp_string_113+4));
char *__tmp_string_112="too";
 __boundcheck_metadata_store((void *)(__tmp_string_112),(void *)(__tmp_string_112+3));
char *__tmp_string_111="one";
 __boundcheck_metadata_store((void *)(__tmp_string_111),(void *)(__tmp_string_111+3));
char *__tmp_string_110="spouse";
 __boundcheck_metadata_store((void *)(__tmp_string_110),(void *)(__tmp_string_110+6));
char *__tmp_string_109="trust";
 __boundcheck_metadata_store((void *)(__tmp_string_109),(void *)(__tmp_string_109+5));
char *__tmp_string_108="support";
 __boundcheck_metadata_store((void *)(__tmp_string_108),(void *)(__tmp_string_108+7));
char *__tmp_string_107="Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_107),(void *)(__tmp_string_107+4));
char *__tmp_string_106="their";
 __boundcheck_metadata_store((void *)(__tmp_string_106),(void *)(__tmp_string_106+5));
char *__tmp_string_105="reasonable";
 __boundcheck_metadata_store((void *)(__tmp_string_105),(void *)(__tmp_string_105+10));
char *__tmp_string_104="size";
 __boundcheck_metadata_store((void *)(__tmp_string_104),(void *)(__tmp_string_104+4));
char *__tmp_string_103="get";
 __boundcheck_metadata_store((void *)(__tmp_string_103),(void *)(__tmp_string_103+3));
char *__tmp_string_102="Politicians";
 __boundcheck_metadata_store((void *)(__tmp_string_102),(void *)(__tmp_string_102+11));
char *__tmp_string_101="Accept";
 __boundcheck_metadata_store((void *)(__tmp_string_101),(void *)(__tmp_string_101+6));
char *__tmp_string_100="before";
 __boundcheck_metadata_store((void *)(__tmp_string_100),(void *)(__tmp_string_100+6));
char *__tmp_string_99="Northern";
 __boundcheck_metadata_store((void *)(__tmp_string_99),(void *)(__tmp_string_99+8));
char *__tmp_string_98="it";
 __boundcheck_metadata_store((void *)(__tmp_string_98),(void *)(__tmp_string_98+2));
char *__tmp_string_97="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_97),(void *)(__tmp_string_97+3));
char *__tmp_string_96="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_96),(void *)(__tmp_string_96+3));
char *__tmp_string_95="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_95),(void *)(__tmp_string_95+3));
char *__tmp_string_94="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_94),(void *)(__tmp_string_94+3));
char *__tmp_string_93="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_93),(void *)(__tmp_string_93+3));
char *__tmp_string_92="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_92),(void *)(__tmp_string_92+3));
char *__tmp_string_91="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_91),(void *)(__tmp_string_91+3));
char *__tmp_string_90="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_90),(void *)(__tmp_string_90+3));
char *__tmp_string_89="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_89),(void *)(__tmp_string_89+3));
char *__tmp_string_88="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_88),(void *)(__tmp_string_88+3));
char *__tmp_string_87="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_87),(void *)(__tmp_string_87+3));
char *__tmp_string_86="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_86),(void *)(__tmp_string_86+3));
char *__tmp_string_85="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_85),(void *)(__tmp_string_85+3));
char *__tmp_string_84="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_84),(void *)(__tmp_string_84+3));
char *__tmp_string_83="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_83),(void *)(__tmp_string_83+3));
char *__tmp_string_82="feel";
 __boundcheck_metadata_store((void *)(__tmp_string_82),(void *)(__tmp_string_82+4));
char *__tmp_string_81="beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_81),(void *)(__tmp_string_81+6));
char *__tmp_string_80="you";
 __boundcheck_metadata_store((void *)(__tmp_string_80),(void *)(__tmp_string_80+3));
char *__tmp_string_79="room";
 __boundcheck_metadata_store((void *)(__tmp_string_79),(void *)(__tmp_string_79+4));
char *__tmp_string_78="nowhere";
 __boundcheck_metadata_store((void *)(__tmp_string_78),(void *)(__tmp_string_78+7));
char *__tmp_string_77="own";
 __boundcheck_metadata_store((void *)(__tmp_string_77),(void *)(__tmp_string_77+3));
char *__tmp_string_76="greatest";
 __boundcheck_metadata_store((void *)(__tmp_string_76),(void *)(__tmp_string_76+8));
char *__tmp_string_75="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_75),(void *)(__tmp_string_75+3));
char *__tmp_string_74="it";
 __boundcheck_metadata_store((void *)(__tmp_string_74),(void *)(__tmp_string_74+2));
char *__tmp_string_73="other";
 __boundcheck_metadata_store((void *)(__tmp_string_73),(void *)(__tmp_string_73+5));
char *__tmp_string_72="be";
 __boundcheck_metadata_store((void *)(__tmp_string_72),(void *)(__tmp_string_72+2));
char *__tmp_string_71="it";
 __boundcheck_metadata_store((void *)(__tmp_string_71),(void *)(__tmp_string_71+2));
char *__tmp_string_70="Enjoy";
 __boundcheck_metadata_store((void *)(__tmp_string_70),(void *)(__tmp_string_70+5));
char *__tmp_string_69="are";
 __boundcheck_metadata_store((void *)(__tmp_string_69),(void *)(__tmp_string_69+3));
char *__tmp_string_68="either";
 __boundcheck_metadata_store((void *)(__tmp_string_68),(void *)(__tmp_string_68+6));
char *__tmp_string_67="much";
 __boundcheck_metadata_store((void *)(__tmp_string_67),(void *)(__tmp_string_67+4));
char *__tmp_string_66="congratulate";
 __boundcheck_metadata_store((void *)(__tmp_string_66),(void *)(__tmp_string_66+12));
char *__tmp_string_65="your";
 __boundcheck_metadata_store((void *)(__tmp_string_65),(void *)(__tmp_string_65+4));
char *__tmp_string_64="the";
 __boundcheck_metadata_store((void *)(__tmp_string_64),(void *)(__tmp_string_64+3));
char *__tmp_string_63="you";
 __boundcheck_metadata_store((void *)(__tmp_string_63),(void *)(__tmp_string_63+3));
char *__tmp_string_62="may";
 __boundcheck_metadata_store((void *)(__tmp_string_62),(void *)(__tmp_string_62+3));
char *__tmp_string_61="Maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_61),(void *)(__tmp_string_61+5));
char *__tmp_string_60="them";
 __boundcheck_metadata_store((void *)(__tmp_string_60),(void *)(__tmp_string_60+4));
char *__tmp_string_59="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_59),(void *)(__tmp_string_59+3));
char *__tmp_string_58="Get";
 __boundcheck_metadata_store((void *)(__tmp_string_58),(void *)(__tmp_string_58+3));
char *__tmp_string_57="40";
 __boundcheck_metadata_store((void *)(__tmp_string_57),(void *)(__tmp_string_57+2));
char *__tmp_string_56="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_56),(void *)(__tmp_string_56+3));
char *__tmp_string_55="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_55),(void *)(__tmp_string_55+3));
char *__tmp_string_54="wha";
 __boundcheck_metadata_store((void *)(__tmp_string_54),(void *)(__tmp_string_54+3));
char *__tmp_string_53="kno";
 __boundcheck_metadata_store((void *)(__tmp_string_53),(void *)(__tmp_string_53+3));
char *__tmp_string_52="life";
 __boundcheck_metadata_store((void *)(__tmp_string_52),(void *)(__tmp_string_52+4));
char *__tmp_string_51="what";
 __boundcheck_metadata_store((void *)(__tmp_string_51),(void *)(__tmp_string_51+4));
char *__tmp_string_50="Stretc";
 __boundcheck_metadata_store((void *)(__tmp_string_50),(void *)(__tmp_string_50+6));
char *__tmp_string_49="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_49),(void *)(__tmp_string_49+2));
char *__tmp_string_48="lov";
 __boundcheck_metadata_store((void *)(__tmp_string_48),(void *)(__tmp_string_48+3));
char *__tmp_string_47="Kee";
 __boundcheck_metadata_store((void *)(__tmp_string_47),(void *)(__tmp_string_47+3));
char *__tmp_string_46="succeed";
 __boundcheck_metadata_store((void *)(__tmp_string_46),(void *)(__tmp_string_46+7));
char *__tmp_string_45="Forget";
 __boundcheck_metadata_store((void *)(__tmp_string_45),(void *)(__tmp_string_45+6));
char *__tmp_string_44="yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_44),(void *)(__tmp_string_44+8));
char *__tmp_string_43="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_43),(void *)(__tmp_string_43+3));
char *__tmp_string_42="Th";
 __boundcheck_metadata_store((void *)(__tmp_string_42),(void *)(__tmp_string_42+2));
char *__tmp_string_41="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_41),(void *)(__tmp_string_41+3));
char *__tmp_string_40="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_40),(void *)(__tmp_string_40+3));
char *__tmp_string_39="wit";
 __boundcheck_metadata_store((void *)(__tmp_string_39),(void *)(__tmp_string_39+3));
char *__tmp_string_38="people";
 __boundcheck_metadata_store((void *)(__tmp_string_38),(void *)(__tmp_string_38+6));
char *__tmp_string_37="scare";
 __boundcheck_metadata_store((void *)(__tmp_string_37),(void *)(__tmp_string_37+5));
char *__tmp_string_36="Do";
 __boundcheck_metadata_store((void *)(__tmp_string_36),(void *)(__tmp_string_36+2));
char *__tmp_string_35="idle";
 __boundcheck_metadata_store((void *)(__tmp_string_35),(void *)(__tmp_string_35+4));
char *__tmp_string_34="4";
 __boundcheck_metadata_store((void *)(__tmp_string_34),(void *)(__tmp_string_34+1));
char *__tmp_string_33="kind";
 __boundcheck_metadata_store((void *)(__tmp_string_33),(void *)(__tmp_string_33+4));
char *__tmp_string_32="wo";
 __boundcheck_metadata_store((void *)(__tmp_string_32),(void *)(__tmp_string_32+2));
char *__tmp_string_31="bxx";
 __boundcheck_metadata_store((void *)(__tmp_string_31),(void *)(__tmp_string_31+3));
char *__tmp_string_30="yo";
 __boundcheck_metadata_store((void *)(__tmp_string_30),(void *)(__tmp_string_30+2));
char *__tmp_string_29="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_29),(void *)(__tmp_string_29+3));
char *__tmp_string_28="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_28),(void *)(__tmp_string_28+3));
char *__tmp_string_27="wor";
 __boundcheck_metadata_store((void *)(__tmp_string_27),(void *)(__tmp_string_27+3));
char *__tmp_string_26="th";
 __boundcheck_metadata_store((void *)(__tmp_string_26),(void *)(__tmp_string_26+2));
char *__tmp_string_25="imag";
 __boundcheck_metadata_store((void *)(__tmp_string_25),(void *)(__tmp_string_25+4));
char *__tmp_string_24="fat";
 __boundcheck_metadata_store((void *)(__tmp_string_24),(void *)(__tmp_string_24+3));
char *__tmp_string_23="an";
 __boundcheck_metadata_store((void *)(__tmp_string_23),(void *)(__tmp_string_23+2));
char *__tmp_string_22="possibili";
 __boundcheck_metadata_store((void *)(__tmp_string_22),(void *)(__tmp_string_22+9));
char *__tmp_string_21="way";
 __boundcheck_metadata_store((void *)(__tmp_string_21),(void *)(__tmp_string_21+3));
char *__tmp_string_20="an";
 __boundcheck_metadata_store((void *)(__tmp_string_20),(void *)(__tmp_string_20+2));
char *__tmp_string_19="bac";
 __boundcheck_metadata_store((void *)(__tmp_string_19),(void *)(__tmp_string_19+3));
char *__tmp_string_18="you";
 __boundcheck_metadata_store((void *)(__tmp_string_18),(void *)(__tmp_string_18+3));
char *__tmp_string_17="20";
 __boundcheck_metadata_store((void *)(__tmp_string_17),(void *)(__tmp_string_17+2));
char *__tmp_string_16="faded";
 __boundcheck_metadata_store((void *)(__tmp_string_16),(void *)(__tmp_string_16+5));
char *__tmp_string_15="and";
 __boundcheck_metadata_store((void *)(__tmp_string_15),(void *)(__tmp_string_15+3));
char *__tmp_string_14="Yo";
 __boundcheck_metadata_store((void *)(__tmp_string_14),(void *)(__tmp_string_14+2));
char *__tmp_string_13="xxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_13),(void *)(__tmp_string_13+5));
char *__tmp_string_12="pow";
 __boundcheck_metadata_store((void *)(__tmp_string_12),(void *)(__tmp_string_12+3));
char *__tmp_string_11="pense";
 __boundcheck_metadata_store((void *)(__tmp_string_11),(void *)(__tmp_string_11+5));
char *__tmp_string_10="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_10),(void *)(__tmp_string_10+6));
char *__tmp_string_9="xxxxxx";
 __boundcheck_metadata_store((void *)(__tmp_string_9),(void *)(__tmp_string_9+6));
char *__tmp_string_8="where";
 __boundcheck_metadata_store((void *)(__tmp_string_8),(void *)(__tmp_string_8+5));
char *__tmp_string_7="have";
 __boundcheck_metadata_store((void *)(__tmp_string_7),(void *)(__tmp_string_7+4));
char *__tmp_string_6="long";
 __boundcheck_metadata_store((void *)(__tmp_string_6),(void *)(__tmp_string_6+4));
char *__tmp_string_5="xxx";
 __boundcheck_metadata_store((void *)(__tmp_string_5),(void *)(__tmp_string_5+3));
char *__tmp_string_4="for";
 __boundcheck_metadata_store((void *)(__tmp_string_4),(void *)(__tmp_string_4+3));
char *__tmp_string_3="suns";
 __boundcheck_metadata_store((void *)(__tmp_string_3),(void *)(__tmp_string_3+4));
char *__tmp_string_2="lass";
 __boundcheck_metadata_store((void *)(__tmp_string_2),(void *)(__tmp_string_2+4));
char *__tmp_string_1="gent";
 __boundcheck_metadata_store((void *)(__tmp_string_1),(void *)(__tmp_string_1+4));
char *__tmp_string_0="Kur";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+3));
char *find_strings[] = { __tmp_string_0,
__tmp_string_1,
__tmp_string_2,
__tmp_string_3,
__tmp_string_4,
__tmp_string_5,
__tmp_string_6,
__tmp_string_7,
__tmp_string_8,
__tmp_string_9,
__tmp_string_10,
__tmp_string_11,
__tmp_string_12,
__tmp_string_13,
__tmp_string_14,
__tmp_string_15,
__tmp_string_16,
__tmp_string_17,
__tmp_string_18,
__tmp_string_19,
__tmp_string_20,
__tmp_string_21,
__tmp_string_22,
__tmp_string_23,
__tmp_string_24,
__tmp_string_25,
__tmp_string_26,
__tmp_string_27,
__tmp_string_28,
__tmp_string_29,
__tmp_string_30,
__tmp_string_31,
__tmp_string_32,
__tmp_string_33,
__tmp_string_34,
__tmp_string_35,
__tmp_string_36,
__tmp_string_37,
__tmp_string_38,
__tmp_string_39,
__tmp_string_40,
__tmp_string_41,
__tmp_string_42,
__tmp_string_43,
__tmp_string_44,
__tmp_string_45,
__tmp_string_46,
__tmp_string_47,
__tmp_string_48,
__tmp_string_49,
__tmp_string_50,
__tmp_string_51,
__tmp_string_52,
__tmp_string_53,
__tmp_string_54,
__tmp_string_55,
__tmp_string_56,
__tmp_string_57,
__tmp_string_58,
__tmp_string_59,
__tmp_string_60,
__tmp_string_61,
__tmp_string_62,
__tmp_string_63,
__tmp_string_64,
__tmp_string_65,
__tmp_string_66,
__tmp_string_67,
__tmp_string_68,
__tmp_string_69,
__tmp_string_70,
__tmp_string_71,
__tmp_string_72,
__tmp_string_73,
__tmp_string_74,
__tmp_string_75,
__tmp_string_76,
__tmp_string_77,
__tmp_string_78,
__tmp_string_79,
__tmp_string_80,
__tmp_string_81,
__tmp_string_82,
__tmp_string_83,
__tmp_string_84,
__tmp_string_85,
__tmp_string_86,
__tmp_string_87,
__tmp_string_88,
__tmp_string_89,
__tmp_string_90,
__tmp_string_91,
__tmp_string_92,
__tmp_string_93,
__tmp_string_94,
__tmp_string_95,
__tmp_string_96,
__tmp_string_97,
__tmp_string_98,
__tmp_string_99,
__tmp_string_100,
__tmp_string_101,
__tmp_string_102,
__tmp_string_103,
__tmp_string_104,
__tmp_string_105,
__tmp_string_106,
__tmp_string_107,
__tmp_string_108,
__tmp_string_109,
__tmp_string_110,
__tmp_string_111,
__tmp_string_112,
__tmp_string_113,
__tmp_string_114,
__tmp_string_115,
__tmp_string_116,
__tmp_string_117,
__tmp_string_118,
__tmp_string_119,
__tmp_string_120,
__tmp_string_121,
__tmp_string_122,
__tmp_string_123,
__tmp_string_124,
__tmp_string_125,
__tmp_string_126,
__tmp_string_127,
__tmp_string_128,
__tmp_string_129,
__tmp_string_130,
__tmp_string_131,
__tmp_string_132,
__tmp_string_133,
__tmp_string_134,
__tmp_string_135,
__tmp_string_136,
__tmp_string_137,
__tmp_string_138,
__tmp_string_139,
__tmp_string_140,
__tmp_string_141,
__tmp_string_142,
__tmp_string_143,
__tmp_string_144,
__tmp_string_145,
__tmp_string_146,
__tmp_string_147,
__tmp_string_148,
__tmp_string_149,
__tmp_string_150,
__tmp_string_151,
__tmp_string_152,
__tmp_string_153,
__tmp_string_154,
__tmp_string_155,
__tmp_string_156,
__tmp_string_157,
__tmp_string_158,
__tmp_string_159,
__tmp_string_160,
__tmp_string_161,
__tmp_string_162,
__tmp_string_163,
__tmp_string_164,
__tmp_string_165,
__tmp_string_166,
__tmp_string_167,
__tmp_string_168,
__tmp_string_169,
__tmp_string_170,
__tmp_string_171,
__tmp_string_172,
__tmp_string_173,
__tmp_string_174,
__tmp_string_175,
__tmp_string_176,
__tmp_string_177,
__tmp_string_178,
__tmp_string_179,
__tmp_string_180,
__tmp_string_181,
__tmp_string_182,
__tmp_string_183,
__tmp_string_184,
__tmp_string_185,
__tmp_string_186,
__tmp_string_187,
__tmp_string_188,
__tmp_string_189,
__tmp_string_190,
__tmp_string_191,
__tmp_string_192,
__tmp_string_193,
__tmp_string_194,
__tmp_string_195,
__tmp_string_196,
__tmp_string_197,
__tmp_string_198,
__tmp_string_199,
__tmp_string_200,
__tmp_string_201,
__tmp_string_202,
__tmp_string_203,
__tmp_string_204,
__tmp_string_205,
__tmp_string_206,
__tmp_string_207,
__tmp_string_208,
__tmp_string_209,
__tmp_string_210,
__tmp_string_211,
__tmp_string_212,
__tmp_string_213,
__tmp_string_214,
__tmp_string_215,
__tmp_string_216,
__tmp_string_217,
__tmp_string_218,
__tmp_string_219,
__tmp_string_220,
__tmp_string_221,
__tmp_string_222,
__tmp_string_223,
__tmp_string_224,
__tmp_string_225,
__tmp_string_226,
__tmp_string_227,
__tmp_string_228,
__tmp_string_229,
__tmp_string_230,
__tmp_string_231,
__tmp_string_232,
__tmp_string_233,
__tmp_string_234,
__tmp_string_235,
__tmp_string_236,
__tmp_string_237,
__tmp_string_238,
__tmp_string_239,
__tmp_string_240,
__tmp_string_241,
__tmp_string_242,
__tmp_string_243,
__tmp_string_244,
__tmp_string_245,
__tmp_string_246,
__tmp_string_247,
__tmp_string_248,
__tmp_string_249,
__tmp_string_250,
__tmp_string_251,
__tmp_string_252,
__tmp_string_253,
__tmp_string_254,
__tmp_string_255,
__tmp_string_256,
__tmp_string_257,
__tmp_string_258,
__tmp_string_259,
__tmp_string_260,
__tmp_string_261,
__tmp_string_262,
__tmp_string_263,
__tmp_string_264,
__tmp_string_265,
__tmp_string_266,
__tmp_string_267,
__tmp_string_268,
__tmp_string_269,
__tmp_string_270,
__tmp_string_271,
__tmp_string_272,
__tmp_string_273,
__tmp_string_274,
__tmp_string_275,
__tmp_string_276,
__tmp_string_277,
__tmp_string_278,
__tmp_string_279,
__tmp_string_280,
__tmp_string_281,
__tmp_string_282,
__tmp_string_283,
__tmp_string_284,
__tmp_string_285,
__tmp_string_286,
__tmp_string_287,
__tmp_string_288,
__tmp_string_289,
__tmp_string_290,
__tmp_string_291,
__tmp_string_292,
__tmp_string_293,
__tmp_string_294,
__tmp_string_295,
__tmp_string_296,
__tmp_string_297,
__tmp_string_298,
__tmp_string_299,
__tmp_string_300,
__tmp_string_301,
__tmp_string_302,
__tmp_string_303,
__tmp_string_304,
__tmp_string_305,
__tmp_string_306,
__tmp_string_307,
__tmp_string_308,
__tmp_string_309,
__tmp_string_310,
__tmp_string_311,
__tmp_string_312,
__tmp_string_313,
__tmp_string_314,
__tmp_string_315,
__tmp_string_316,
__tmp_string_317,
__tmp_string_318,
__tmp_string_319,
__tmp_string_320,
__tmp_string_321,
__tmp_string_322,
__tmp_string_323,
__tmp_string_324,
__tmp_string_325,
__tmp_string_326,
__tmp_string_327,
__tmp_string_328,
__tmp_string_329,
__tmp_string_330,
__tmp_string_331,
__tmp_string_332,
__tmp_string_333,
__tmp_string_334,
__tmp_string_335,
__tmp_string_336,
__tmp_string_337,
__tmp_string_338,
__tmp_string_339,
__tmp_string_340,
__tmp_string_341,
__tmp_string_342,
__tmp_string_343,
__tmp_string_344,
__tmp_string_345,
__tmp_string_346,
__tmp_string_347,
__tmp_string_348,
__tmp_string_349,
__tmp_string_350,
__tmp_string_351,
__tmp_string_352,
__tmp_string_353,
__tmp_string_354,
__tmp_string_355,
__tmp_string_356,
__tmp_string_357,
__tmp_string_358,
__tmp_string_359,
__tmp_string_360,
__tmp_string_361,
__tmp_string_362,
__tmp_string_363,
__tmp_string_364,
__tmp_string_365,
__tmp_string_366,
__tmp_string_367,
__tmp_string_368,
__tmp_string_369,
__tmp_string_370,
__tmp_string_371,
__tmp_string_372,
__tmp_string_373,
__tmp_string_374,
__tmp_string_375,
__tmp_string_376,
__tmp_string_377,
__tmp_string_378,
__tmp_string_379,
__tmp_string_380,
__tmp_string_381,
__tmp_string_382,
__tmp_string_383,
__tmp_string_384,
__tmp_string_385,
__tmp_string_386,
__tmp_string_387,
__tmp_string_388,
__tmp_string_389,
__tmp_string_390,
__tmp_string_391,
__tmp_string_392,
__tmp_string_393,
__tmp_string_394,
__tmp_string_395,
__tmp_string_396,
__tmp_string_397,
__tmp_string_398,
__tmp_string_399,
__tmp_string_400,
__tmp_string_401,
__tmp_string_402,
__tmp_string_403,
__tmp_string_404,
__tmp_string_405,
__tmp_string_406,
__tmp_string_407,
__tmp_string_408,
__tmp_string_409,
__tmp_string_410,
__tmp_string_411,
__tmp_string_412,
__tmp_string_413,
__tmp_string_414,
__tmp_string_415,
__tmp_string_416,
__tmp_string_417,
__tmp_string_418,
__tmp_string_419,
__tmp_string_420,
__tmp_string_421,
__tmp_string_422,
__tmp_string_423,
__tmp_string_424,
__tmp_string_425,
__tmp_string_426,
__tmp_string_427,
__tmp_string_428,
__tmp_string_429,
__tmp_string_430,
__tmp_string_431,
__tmp_string_432,
__tmp_string_433,
__tmp_string_434,
__tmp_string_435,
__tmp_string_436,
__tmp_string_437,
__tmp_string_438,
__tmp_string_439,
__tmp_string_440,
__tmp_string_441,
__tmp_string_442,
__tmp_string_443,
__tmp_string_444,
__tmp_string_445,
__tmp_string_446,
__tmp_string_447,
__tmp_string_448,
__tmp_string_449,
__tmp_string_450,
__tmp_string_451,
__tmp_string_452,
__tmp_string_453,
__tmp_string_454,
__tmp_string_455,
__tmp_string_456,
__tmp_string_457,
__tmp_string_458,
__tmp_string_459,
__tmp_string_460,
__tmp_string_461,
__tmp_string_462,
__tmp_string_463,
__tmp_string_464,
__tmp_string_465,
__tmp_string_466,
__tmp_string_467,
__tmp_string_468,
__tmp_string_469,
__tmp_string_470,
__tmp_string_471,
__tmp_string_472,
__tmp_string_473,
__tmp_string_474,
__tmp_string_475,
__tmp_string_476,
__tmp_string_477,
__tmp_string_478,
__tmp_string_479,
__tmp_string_480,
__tmp_string_481,
__tmp_string_482,
__tmp_string_483,
__tmp_string_484,
__tmp_string_485,
__tmp_string_486,
__tmp_string_487,
__tmp_string_488,
__tmp_string_489,
__tmp_string_490,
__tmp_string_491,
__tmp_string_492,
__tmp_string_493,
__tmp_string_494,
__tmp_string_495,
__tmp_string_496,
__tmp_string_497,
__tmp_string_498,
__tmp_string_499,
__tmp_string_500,
__tmp_string_501,
__tmp_string_502,
__tmp_string_503,
__tmp_string_504,
__tmp_string_505,
__tmp_string_506,
__tmp_string_507,
__tmp_string_508,
__tmp_string_509,
__tmp_string_510,
__tmp_string_511,
__tmp_string_512,
__tmp_string_513,
__tmp_string_514,
__tmp_string_515,
__tmp_string_516,
__tmp_string_517,
__tmp_string_518,
__tmp_string_519,
__tmp_string_520,
__tmp_string_521,
__tmp_string_522,
__tmp_string_523,
__tmp_string_524,
__tmp_string_525,
__tmp_string_526,
__tmp_string_527,
__tmp_string_528,
__tmp_string_529,
__tmp_string_530,
__tmp_string_531,
__tmp_string_532,
__tmp_string_533,
__tmp_string_534,
__tmp_string_535,
__tmp_string_536,
__tmp_string_537,
__tmp_string_538,
__tmp_string_539,
__tmp_string_540,
__tmp_string_541,
__tmp_string_542,
__tmp_string_543,
__tmp_string_544,
__tmp_string_545,
__tmp_string_546,
__tmp_string_547,
__tmp_string_548,
__tmp_string_549,
__tmp_string_550,
__tmp_string_551,
__tmp_string_552,
__tmp_string_553,
__tmp_string_554,
__tmp_string_555,
__tmp_string_556,
__tmp_string_557,
__tmp_string_558,
__tmp_string_559,
__tmp_string_560,
__tmp_string_561,
__tmp_string_562,
__tmp_string_563,
__tmp_string_564,
__tmp_string_565,
__tmp_string_566,
__tmp_string_567,
__tmp_string_568,
__tmp_string_569,
__tmp_string_570,
__tmp_string_571,
__tmp_string_572,
__tmp_string_573,
__tmp_string_574,
__tmp_string_575,
__tmp_string_576,
__tmp_string_577,
__tmp_string_578,
__tmp_string_579,
__tmp_string_580,
__tmp_string_581,
__tmp_string_582,
__tmp_string_583,
__tmp_string_584,
__tmp_string_585,
__tmp_string_586,
__tmp_string_587,
__tmp_string_588,
__tmp_string_589,
__tmp_string_590,
__tmp_string_591,
__tmp_string_592,
__tmp_string_593,
__tmp_string_594,
__tmp_string_595,
__tmp_string_596,
__tmp_string_597,
__tmp_string_598,
__tmp_string_599,
__tmp_string_600,
__tmp_string_601,
__tmp_string_602,
__tmp_string_603,
__tmp_string_604,
__tmp_string_605,
__tmp_string_606,
__tmp_string_607,
__tmp_string_608,
__tmp_string_609,
__tmp_string_610,
__tmp_string_611,
__tmp_string_612,
__tmp_string_613,
__tmp_string_614,
__tmp_string_615,
__tmp_string_616,
__tmp_string_617,
__tmp_string_618,
__tmp_string_619,
__tmp_string_620,
__tmp_string_621,
__tmp_string_622,
__tmp_string_623,
__tmp_string_624,
__tmp_string_625,
__tmp_string_626,
__tmp_string_627,
__tmp_string_628,
__tmp_string_629,
__tmp_string_630,
__tmp_string_631,
__tmp_string_632,
__tmp_string_633,
__tmp_string_634,
__tmp_string_635,
__tmp_string_636,
__tmp_string_637,
__tmp_string_638,
__tmp_string_639,
__tmp_string_640,
__tmp_string_641,
__tmp_string_642,
__tmp_string_643,
__tmp_string_644,
__tmp_string_645,
__tmp_string_646,
__tmp_string_647,
__tmp_string_648,
__tmp_string_649,
__tmp_string_650,
__tmp_string_651,
__tmp_string_652,
__tmp_string_653,
__tmp_string_654,
__tmp_string_655,
__tmp_string_656,
__tmp_string_657,
__tmp_string_658,
__tmp_string_659,
__tmp_string_660,
__tmp_string_661,
__tmp_string_662,
__tmp_string_663,
__tmp_string_664,
__tmp_string_665,
__tmp_string_666,
__tmp_string_667,
__tmp_string_668,
__tmp_string_669,
__tmp_string_670,
__tmp_string_671,
__tmp_string_672,
__tmp_string_673,
__tmp_string_674,
__tmp_string_675,
__tmp_string_676,
__tmp_string_677,
__tmp_string_678,
__tmp_string_679,
__tmp_string_680,
__tmp_string_681,
__tmp_string_682,
__tmp_string_683,
__tmp_string_684,
__tmp_string_685,
__tmp_string_686,
__tmp_string_687,
__tmp_string_688,
__tmp_string_689,
__tmp_string_690,
__tmp_string_691,
__tmp_string_692,
__tmp_string_693,
__tmp_string_694,
__tmp_string_695,
__tmp_string_696,
__tmp_string_697,
__tmp_string_698,
__tmp_string_699,
__tmp_string_700,
__tmp_string_701,
__tmp_string_702,
__tmp_string_703,
__tmp_string_704,
__tmp_string_705,
__tmp_string_706,
__tmp_string_707,
__tmp_string_708,
__tmp_string_709,
__tmp_string_710,
__tmp_string_711,
__tmp_string_712,
__tmp_string_713,
__tmp_string_714,
__tmp_string_715,
__tmp_string_716,
__tmp_string_717,
__tmp_string_718,
__tmp_string_719,
__tmp_string_720,
__tmp_string_721,
__tmp_string_722,
__tmp_string_723,
__tmp_string_724,
__tmp_string_725,
__tmp_string_726,
__tmp_string_727,
__tmp_string_728,
__tmp_string_729,
__tmp_string_730,
__tmp_string_731,
__tmp_string_732,
__tmp_string_733,
__tmp_string_734,
__tmp_string_735,
__tmp_string_736,
__tmp_string_737,
__tmp_string_738,
__tmp_string_739,
__tmp_string_740,
__tmp_string_741,
__tmp_string_742,
__tmp_string_743,
__tmp_string_744,
__tmp_string_745,
__tmp_string_746,
__tmp_string_747,
__tmp_string_748,
__tmp_string_749,
__tmp_string_750,
__tmp_string_751,
__tmp_string_752,
__tmp_string_753,
__tmp_string_754,
__tmp_string_755,
__tmp_string_756,
__tmp_string_757,
__tmp_string_758,
__tmp_string_759,
__tmp_string_760,
__tmp_string_761,
__tmp_string_762,
__tmp_string_763,
__tmp_string_764,
__tmp_string_765,
__tmp_string_766,
__tmp_string_767,
__tmp_string_768,
__tmp_string_769,
__tmp_string_770,
__tmp_string_771,
__tmp_string_772,
__tmp_string_773,
__tmp_string_774,
__tmp_string_775,
__tmp_string_776,
__tmp_string_777,
__tmp_string_778,
__tmp_string_779,
__tmp_string_780,
__tmp_string_781,
__tmp_string_782,
__tmp_string_783,
__tmp_string_784,
__tmp_string_785,
__tmp_string_786,
__tmp_string_787,
__tmp_string_788,
__tmp_string_789,
__tmp_string_790,
__tmp_string_791,
__tmp_string_792,
__tmp_string_793,
__tmp_string_794,
__tmp_string_795,
__tmp_string_796,
__tmp_string_797,
__tmp_string_798,
__tmp_string_799,
__tmp_string_800,
__tmp_string_801,
__tmp_string_802,
__tmp_string_803,
__tmp_string_804,
__tmp_string_805,
__tmp_string_806,
__tmp_string_807,
__tmp_string_808,
__tmp_string_809,
__tmp_string_810,
__tmp_string_811,
__tmp_string_812,
__tmp_string_813,
__tmp_string_814,
__tmp_string_815,
__tmp_string_816,
__tmp_string_817,
__tmp_string_818,
__tmp_string_819,
__tmp_string_820,
__tmp_string_821,
__tmp_string_822,
__tmp_string_823,
__tmp_string_824,
__tmp_string_825,
__tmp_string_826,
__tmp_string_827,
__tmp_string_828,
__tmp_string_829,
__tmp_string_830,
__tmp_string_831,
__tmp_string_832,
__tmp_string_833,
__tmp_string_834,
__tmp_string_835,
__tmp_string_836,
__tmp_string_837,
__tmp_string_838,
__tmp_string_839,
__tmp_string_840,
__tmp_string_841,
__tmp_string_842,
__tmp_string_843,
__tmp_string_844,
__tmp_string_845,
__tmp_string_846,
__tmp_string_847,
__tmp_string_848,
__tmp_string_849,
__tmp_string_850,
__tmp_string_851,
__tmp_string_852,
__tmp_string_853,
__tmp_string_854,
__tmp_string_855,
__tmp_string_856,
__tmp_string_857,
__tmp_string_858,
__tmp_string_859,
__tmp_string_860,
__tmp_string_861,
__tmp_string_862,
__tmp_string_863,
__tmp_string_864,
__tmp_string_865,
__tmp_string_866,
__tmp_string_867,
__tmp_string_868,
__tmp_string_869,
__tmp_string_870,
__tmp_string_871,
__tmp_string_872,
__tmp_string_873,
__tmp_string_874,
__tmp_string_875,
__tmp_string_876,
__tmp_string_877,
__tmp_string_878,
__tmp_string_879,
__tmp_string_880,
__tmp_string_881,
__tmp_string_882,
__tmp_string_883,
__tmp_string_884,
__tmp_string_885,
__tmp_string_886,
__tmp_string_887,
__tmp_string_888,
__tmp_string_889,
__tmp_string_890,
__tmp_string_891,
__tmp_string_892,
__tmp_string_893,
__tmp_string_894,
__tmp_string_895,
__tmp_string_896,
__tmp_string_897,
__tmp_string_898,
__tmp_string_899,
__tmp_string_900,
__tmp_string_901,
__tmp_string_902,
__tmp_string_903,
__tmp_string_904,
__tmp_string_905,
__tmp_string_906,
__tmp_string_907,
__tmp_string_908,
__tmp_string_909,
__tmp_string_910,
__tmp_string_911,
__tmp_string_912,
__tmp_string_913,
__tmp_string_914,
__tmp_string_915,
__tmp_string_916,
__tmp_string_917,
__tmp_string_918,
__tmp_string_919,
__tmp_string_920,
__tmp_string_921,
__tmp_string_922,
__tmp_string_923,
__tmp_string_924,
__tmp_string_925,
__tmp_string_926,
__tmp_string_927,
__tmp_string_928,
__tmp_string_929,
__tmp_string_930,
__tmp_string_931,
__tmp_string_932,
__tmp_string_933,
__tmp_string_934,
__tmp_string_935,
__tmp_string_936,
__tmp_string_937,
__tmp_string_938,
__tmp_string_939,
__tmp_string_940,
__tmp_string_941,
__tmp_string_942,
__tmp_string_943,
__tmp_string_944,
__tmp_string_945,
__tmp_string_946,
__tmp_string_947,
__tmp_string_948,
__tmp_string_949,
__tmp_string_950,
__tmp_string_951,
__tmp_string_952,
__tmp_string_953,
__tmp_string_954,
__tmp_string_955,
__tmp_string_956,
__tmp_string_957,
__tmp_string_958,
__tmp_string_959,
__tmp_string_960,
__tmp_string_961,
__tmp_string_962,
__tmp_string_963,
__tmp_string_964,
__tmp_string_965,
__tmp_string_966,
__tmp_string_967,
__tmp_string_968,
__tmp_string_969,
__tmp_string_970,
__tmp_string_971,
__tmp_string_972,
__tmp_string_973,
__tmp_string_974,
__tmp_string_975,
__tmp_string_976,
__tmp_string_977,
__tmp_string_978,
__tmp_string_979,
__tmp_string_980,
__tmp_string_981,
__tmp_string_982,
__tmp_string_983,
__tmp_string_984,
__tmp_string_985,
__tmp_string_986,
__tmp_string_987,
__tmp_string_988,
__tmp_string_989,
__tmp_string_990,
__tmp_string_991,
__tmp_string_992,
__tmp_string_993,
__tmp_string_994,
__tmp_string_995,
__tmp_string_996,
__tmp_string_997,
__tmp_string_998,
__tmp_string_999,
__tmp_string_1000,
__tmp_string_1001,
__tmp_string_1002,
__tmp_string_1003,
__tmp_string_1004,
__tmp_string_1005,
__tmp_string_1006,
__tmp_string_1007,
__tmp_string_1008,
__tmp_string_1009,
__tmp_string_1010,
__tmp_string_1011,
__tmp_string_1012,
__tmp_string_1013,
__tmp_string_1014,
__tmp_string_1015,
__tmp_string_1016,
__tmp_string_1017,
__tmp_string_1018,
__tmp_string_1019,
__tmp_string_1020,
__tmp_string_1021,
__tmp_string_1022,
__tmp_string_1023,
__tmp_string_1024,
__tmp_string_1025,
__tmp_string_1026,
__tmp_string_1027,
__tmp_string_1028,
__tmp_string_1029,
__tmp_string_1030,
__tmp_string_1031,
__tmp_string_1032,
__tmp_string_1033,
__tmp_string_1034,
__tmp_string_1035,
__tmp_string_1036,
__tmp_string_1037,
__tmp_string_1038,
__tmp_string_1039,
__tmp_string_1040,
__tmp_string_1041,
__tmp_string_1042,
__tmp_string_1043,
__tmp_string_1044,
__tmp_string_1045,
__tmp_string_1046,
__tmp_string_1047,
__tmp_string_1048,
__tmp_string_1049,
__tmp_string_1050,
__tmp_string_1051,
__tmp_string_1052,
__tmp_string_1053,
__tmp_string_1054,
__tmp_string_1055,
__tmp_string_1056,
__tmp_string_1057,
__tmp_string_1058,
__tmp_string_1059,
__tmp_string_1060,
__tmp_string_1061,
__tmp_string_1062,
__tmp_string_1063,
__tmp_string_1064,
__tmp_string_1065,
__tmp_string_1066,
__tmp_string_1067,
__tmp_string_1068,
__tmp_string_1069,
__tmp_string_1070,
__tmp_string_1071,
__tmp_string_1072,
__tmp_string_1073,
__tmp_string_1074,
__tmp_string_1075,
__tmp_string_1076,
__tmp_string_1077,
__tmp_string_1078,
__tmp_string_1079,
__tmp_string_1080,
__tmp_string_1081,
__tmp_string_1082,
__tmp_string_1083,
__tmp_string_1084,
__tmp_string_1085,
__tmp_string_1086,
__tmp_string_1087,
__tmp_string_1088,
__tmp_string_1089,
__tmp_string_1090,
__tmp_string_1091,
__tmp_string_1092,
__tmp_string_1093,
__tmp_string_1094,
__tmp_string_1095,
__tmp_string_1096,
__tmp_string_1097,
__tmp_string_1098,
__tmp_string_1099,
__tmp_string_1100,
__tmp_string_1101,
__tmp_string_1102,
__tmp_string_1103,
__tmp_string_1104,
__tmp_string_1105,
__tmp_string_1106,
__tmp_string_1107,
__tmp_string_1108,
__tmp_string_1109,
__tmp_string_1110,
__tmp_string_1111,
__tmp_string_1112,
__tmp_string_1113,
__tmp_string_1114,
__tmp_string_1115,
__tmp_string_1116,
__tmp_string_1117,
__tmp_string_1118,
__tmp_string_1119,
__tmp_string_1120,
__tmp_string_1121,
__tmp_string_1122,
__tmp_string_1123,
__tmp_string_1124,
__tmp_string_1125,
__tmp_string_1126,
__tmp_string_1127,
__tmp_string_1128,
__tmp_string_1129,
__tmp_string_1130,
__tmp_string_1131,
__tmp_string_1132,
__tmp_string_1133,
__tmp_string_1134,
__tmp_string_1135,
__tmp_string_1136,
__tmp_string_1137,
__tmp_string_1138,
__tmp_string_1139,
__tmp_string_1140,
__tmp_string_1141,
__tmp_string_1142,
__tmp_string_1143,
__tmp_string_1144,
__tmp_string_1145,
__tmp_string_1146,
__tmp_string_1147,
__tmp_string_1148,
__tmp_string_1149,
__tmp_string_1150,
__tmp_string_1151,
__tmp_string_1152,
__tmp_string_1153,
__tmp_string_1154,
__tmp_string_1155,
__tmp_string_1156,
__tmp_string_1157,
__tmp_string_1158,
__tmp_string_1159,
__tmp_string_1160,
__tmp_string_1161,
__tmp_string_1162,
__tmp_string_1163,
__tmp_string_1164,
__tmp_string_1165,
__tmp_string_1166,
__tmp_string_1167,
__tmp_string_1168,
__tmp_string_1169,
__tmp_string_1170,
__tmp_string_1171,
__tmp_string_1172,
__tmp_string_1173,
__tmp_string_1174,
__tmp_string_1175,
__tmp_string_1176,
__tmp_string_1177,
__tmp_string_1178,
__tmp_string_1179,
__tmp_string_1180,
__tmp_string_1181,
__tmp_string_1182,
__tmp_string_1183,
__tmp_string_1184,
__tmp_string_1185,
__tmp_string_1186,
__tmp_string_1187,
__tmp_string_1188,
__tmp_string_1189,
__tmp_string_1190,
__tmp_string_1191,
__tmp_string_1192,
__tmp_string_1193,
__tmp_string_1194,
__tmp_string_1195,
__tmp_string_1196,
__tmp_string_1197,
__tmp_string_1198,
__tmp_string_1199,
__tmp_string_1200,
__tmp_string_1201,
__tmp_string_1202,
__tmp_string_1203,
__tmp_string_1204,
__tmp_string_1205,
__tmp_string_1206,
__tmp_string_1207,
__tmp_string_1208,
__tmp_string_1209,
__tmp_string_1210,
__tmp_string_1211,
__tmp_string_1212,
__tmp_string_1213,
__tmp_string_1214,
__tmp_string_1215,
__tmp_string_1216,
__tmp_string_1217,
__tmp_string_1218,
__tmp_string_1219,
__tmp_string_1220,
__tmp_string_1221,
__tmp_string_1222,
__tmp_string_1223,
__tmp_string_1224,
__tmp_string_1225,
__tmp_string_1226,
__tmp_string_1227,
__tmp_string_1228,
__tmp_string_1229,
__tmp_string_1230,
__tmp_string_1231,
__tmp_string_1232,
__tmp_string_1233,
__tmp_string_1234,
__tmp_string_1235,
__tmp_string_1236,
__tmp_string_1237,
__tmp_string_1238,
__tmp_string_1239,
__tmp_string_1240,
__tmp_string_1241,
__tmp_string_1242,
__tmp_string_1243,
__tmp_string_1244,
__tmp_string_1245,
__tmp_string_1246,
__tmp_string_1247,
__tmp_string_1248,
__tmp_string_1249,
__tmp_string_1250,
__tmp_string_1251,
__tmp_string_1252,
__tmp_string_1253,
__tmp_string_1254,
__tmp_string_1255,
__tmp_string_1256,
__tmp_string_1257,
__tmp_string_1258,
__tmp_string_1259,
__tmp_string_1260,
__tmp_string_1261,
__tmp_string_1262,
__tmp_string_1263,
__tmp_string_1264,
__tmp_string_1265,
__tmp_string_1266,
__tmp_string_1267,
__tmp_string_1268,
__tmp_string_1269,
__tmp_string_1270,
__tmp_string_1271,
__tmp_string_1272,
__tmp_string_1273,
__tmp_string_1274,
__tmp_string_1275,
__tmp_string_1276,
__tmp_string_1277,
__tmp_string_1278,
__tmp_string_1279,
__tmp_string_1280,
__tmp_string_1281,
__tmp_string_1282,
__tmp_string_1283,
__tmp_string_1284,
__tmp_string_1285,
__tmp_string_1286,
__tmp_string_1287,
__tmp_string_1288,
__tmp_string_1289,
__tmp_string_1290,
__tmp_string_1291,
__tmp_string_1292,
__tmp_string_1293,
__tmp_string_1294,
__tmp_string_1295,
__tmp_string_1296,
__tmp_string_1297,
__tmp_string_1298,
__tmp_string_1299,
__tmp_string_1300,
__tmp_string_1301,
__tmp_string_1302,
__tmp_string_1303,
__tmp_string_1304,
__tmp_string_1305,
__tmp_string_1306,
__tmp_string_1307,
__tmp_string_1308,
__tmp_string_1309,
__tmp_string_1310,
__tmp_string_1311,
__tmp_string_1312,
__tmp_string_1313,
__tmp_string_1314,
__tmp_string_1315,
__tmp_string_1316,
__tmp_string_1317,
__tmp_string_1318,
__tmp_string_1319,
__tmp_string_1320,
__tmp_string_1321,
__tmp_string_1322,
__tmp_string_1323,
__tmp_string_1324,
__tmp_string_1325,
__tmp_string_1326,
__tmp_string_1327,
__tmp_string_1328,
__tmp_string_1329,
__tmp_string_1330,
__tmp_string_1331,
NULL};
__boundcheck_metadata_store((void *)(&find_strings),(void *)((size_t)(&find_strings)+sizeof(find_strings)*8-1));

      char *__tmp_string_2663="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_2663),(void *)(__tmp_string_2663+35));
char *__tmp_string_2662="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_2662),(void *)(__tmp_string_2662+34));
char *__tmp_string_2661="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_2661),(void *)(__tmp_string_2661+19));
char *__tmp_string_2660="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_2660),(void *)(__tmp_string_2660+35));
char *__tmp_string_2659="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_2659),(void *)(__tmp_string_2659+30));
char *__tmp_string_2658="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_2658),(void *)(__tmp_string_2658+34));
char *__tmp_string_2657="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_2657),(void *)(__tmp_string_2657+36));
char *__tmp_string_2656="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_2656),(void *)(__tmp_string_2656+43));
char *__tmp_string_2655="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2655),(void *)(__tmp_string_2655+32));
char *__tmp_string_2654="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_2654),(void *)(__tmp_string_2654+33));
char *__tmp_string_2653="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_2653),(void *)(__tmp_string_2653+27));
char *__tmp_string_2652="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_2652),(void *)(__tmp_string_2652+37));
char *__tmp_string_2651="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_2651),(void *)(__tmp_string_2651+37));
char *__tmp_string_2650="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_2650),(void *)(__tmp_string_2650+28));
char *__tmp_string_2649="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_2649),(void *)(__tmp_string_2649+26));
char *__tmp_string_2648="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_2648),(void *)(__tmp_string_2648+32));
char *__tmp_string_2647="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_2647),(void *)(__tmp_string_2647+56));
char *__tmp_string_2646="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_2646),(void *)(__tmp_string_2646+46));
char *__tmp_string_2645="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_2645),(void *)(__tmp_string_2645+29));
char *__tmp_string_2644="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_2644),(void *)(__tmp_string_2644+39));
char *__tmp_string_2643="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_2643),(void *)(__tmp_string_2643+50));
char *__tmp_string_2642="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_2642),(void *)(__tmp_string_2642+31));
char *__tmp_string_2641="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_2641),(void *)(__tmp_string_2641+34));
char *__tmp_string_2640="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_2640),(void *)(__tmp_string_2640+25));
char *__tmp_string_2639="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_2639),(void *)(__tmp_string_2639+38));
char *__tmp_string_2638="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_2638),(void *)(__tmp_string_2638+15));
char *__tmp_string_2637="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_2637),(void *)(__tmp_string_2637+28));
char *__tmp_string_2636="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_2636),(void *)(__tmp_string_2636+29));
char *__tmp_string_2635="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_2635),(void *)(__tmp_string_2635+17));
char *__tmp_string_2634="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_2634),(void *)(__tmp_string_2634+35));
char *__tmp_string_2633="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_2633),(void *)(__tmp_string_2633+22));
char *__tmp_string_2632="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_2632),(void *)(__tmp_string_2632+35));
char *__tmp_string_2631="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_2631),(void *)(__tmp_string_2631+23));
char *__tmp_string_2630="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_2630),(void *)(__tmp_string_2630+29));
char *__tmp_string_2629="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_2629),(void *)(__tmp_string_2629+17));
char *__tmp_string_2628="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_2628),(void *)(__tmp_string_2628+31));
char *__tmp_string_2627="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_2627),(void *)(__tmp_string_2627+17));
char *__tmp_string_2626="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_2626),(void *)(__tmp_string_2626+33));
char *__tmp_string_2625="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2625),(void *)(__tmp_string_2625+36));
char *__tmp_string_2624="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_2624),(void *)(__tmp_string_2624+44));
char *__tmp_string_2623="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_2623),(void *)(__tmp_string_2623+40));
char *__tmp_string_2622="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_2622),(void *)(__tmp_string_2622+32));
char *__tmp_string_2621="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_2621),(void *)(__tmp_string_2621+32));
char *__tmp_string_2620="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_2620),(void *)(__tmp_string_2620+32));
char *__tmp_string_2619="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_2619),(void *)(__tmp_string_2619+26));
char *__tmp_string_2618="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_2618),(void *)(__tmp_string_2618+30));
char *__tmp_string_2617="the";
 __boundcheck_metadata_store((void *)(__tmp_string_2617),(void *)(__tmp_string_2617+3));
char *__tmp_string_2616="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_2616),(void *)(__tmp_string_2616+6));
char *__tmp_string_2615="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_2615),(void *)(__tmp_string_2615+21));
char *__tmp_string_2614="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_2614),(void *)(__tmp_string_2614+26));
char *__tmp_string_2613="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2613),(void *)(__tmp_string_2613+25));
char *__tmp_string_2612="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_2612),(void *)(__tmp_string_2612+19));
char *__tmp_string_2611="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_2611),(void *)(__tmp_string_2611+19));
char *__tmp_string_2610="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_2610),(void *)(__tmp_string_2610+38));
char *__tmp_string_2609="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2609),(void *)(__tmp_string_2609+23));
char *__tmp_string_2608="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_2608),(void *)(__tmp_string_2608+37));
char *__tmp_string_2607="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_2607),(void *)(__tmp_string_2607+38));
char *__tmp_string_2606="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_2606),(void *)(__tmp_string_2606+20));
char *__tmp_string_2605="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_2605),(void *)(__tmp_string_2605+52));
char *__tmp_string_2604="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_2604),(void *)(__tmp_string_2604+46));
char *__tmp_string_2603="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_2603),(void *)(__tmp_string_2603+17));
char *__tmp_string_2602="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_2602),(void *)(__tmp_string_2602+21));
char *__tmp_string_2601="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_2601),(void *)(__tmp_string_2601+32));
char *__tmp_string_2600="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_2600),(void *)(__tmp_string_2600+21));
char *__tmp_string_2599="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2599),(void *)(__tmp_string_2599+28));
char *__tmp_string_2598="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_2598),(void *)(__tmp_string_2598+20));
char *__tmp_string_2597="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_2597),(void *)(__tmp_string_2597+30));
char *__tmp_string_2596="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_2596),(void *)(__tmp_string_2596+16));
char *__tmp_string_2595="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_2595),(void *)(__tmp_string_2595+21));
char *__tmp_string_2594="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_2594),(void *)(__tmp_string_2594+34));
char *__tmp_string_2593="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_2593),(void *)(__tmp_string_2593+29));
char *__tmp_string_2592="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_2592),(void *)(__tmp_string_2592+41));
char *__tmp_string_2591="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_2591),(void *)(__tmp_string_2591+24));
char *__tmp_string_2590="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_2590),(void *)(__tmp_string_2590+23));
char *__tmp_string_2589="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_2589),(void *)(__tmp_string_2589+13));
char *__tmp_string_2588="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_2588),(void *)(__tmp_string_2588+33));
char *__tmp_string_2587="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_2587),(void *)(__tmp_string_2587+25));
char *__tmp_string_2586="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_2586),(void *)(__tmp_string_2586+41));
char *__tmp_string_2585="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_2585),(void *)(__tmp_string_2585+21));
char *__tmp_string_2584="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_2584),(void *)(__tmp_string_2584+23));
char *__tmp_string_2583="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_2583),(void *)(__tmp_string_2583+56));
char *__tmp_string_2582="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_2582),(void *)(__tmp_string_2582+37));
char *__tmp_string_2581="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_2581),(void *)(__tmp_string_2581+28));
char *__tmp_string_2580="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_2580),(void *)(__tmp_string_2580+26));
char *__tmp_string_2579="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_2579),(void *)(__tmp_string_2579+43));
char *__tmp_string_2578="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_2578),(void *)(__tmp_string_2578+27));
char *__tmp_string_2577="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_2577),(void *)(__tmp_string_2577+12));
char *__tmp_string_2576="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_2576),(void *)(__tmp_string_2576+12));
char *__tmp_string_2575="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_2575),(void *)(__tmp_string_2575+26));
char *__tmp_string_2574="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_2574),(void *)(__tmp_string_2574+16));
char *__tmp_string_2573="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_2573),(void *)(__tmp_string_2573+33));
char *__tmp_string_2572="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_2572),(void *)(__tmp_string_2572+20));
char *__tmp_string_2571="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_2571),(void *)(__tmp_string_2571+42));
char *__tmp_string_2570="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_2570),(void *)(__tmp_string_2570+38));
char *__tmp_string_2569="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_2569),(void *)(__tmp_string_2569+44));
char *__tmp_string_2568="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_2568),(void *)(__tmp_string_2568+13));
char *__tmp_string_2567="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_2567),(void *)(__tmp_string_2567+31));
char *__tmp_string_2566="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_2566),(void *)(__tmp_string_2566+14));
char *__tmp_string_2565="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_2565),(void *)(__tmp_string_2565+38));
char *__tmp_string_2564="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_2564),(void *)(__tmp_string_2564+26));
char *__tmp_string_2563="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_2563),(void *)(__tmp_string_2563+31));
char *__tmp_string_2562="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_2562),(void *)(__tmp_string_2562+15));
char *__tmp_string_2561="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2561),(void *)(__tmp_string_2561+26));
char *__tmp_string_2560="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_2560),(void *)(__tmp_string_2560+10));
char *__tmp_string_2559="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_2559),(void *)(__tmp_string_2559+8));
char *__tmp_string_2558="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_2558),(void *)(__tmp_string_2558+21));
char *__tmp_string_2557="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_2557),(void *)(__tmp_string_2557+37));
char *__tmp_string_2556="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_2556),(void *)(__tmp_string_2556+33));
char *__tmp_string_2555="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_2555),(void *)(__tmp_string_2555+27));
char *__tmp_string_2554="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_2554),(void *)(__tmp_string_2554+30));
char *__tmp_string_2553="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_2553),(void *)(__tmp_string_2553+27));
char *__tmp_string_2552="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_2552),(void *)(__tmp_string_2552+47));
char *__tmp_string_2551="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_2551),(void *)(__tmp_string_2551+22));
char *__tmp_string_2550="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_2550),(void *)(__tmp_string_2550+30));
char *__tmp_string_2549="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_2549),(void *)(__tmp_string_2549+29));
char *__tmp_string_2548="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_2548),(void *)(__tmp_string_2548+27));
char *__tmp_string_2547="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_2547),(void *)(__tmp_string_2547+29));
char *__tmp_string_2546="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_2546),(void *)(__tmp_string_2546+20));
char *__tmp_string_2545="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_2545),(void *)(__tmp_string_2545+26));
char *__tmp_string_2544="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_2544),(void *)(__tmp_string_2544+20));
char *__tmp_string_2543="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_2543),(void *)(__tmp_string_2543+27));
char *__tmp_string_2542="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_2542),(void *)(__tmp_string_2542+35));
char *__tmp_string_2541="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_2541),(void *)(__tmp_string_2541+34));
char *__tmp_string_2540="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_2540),(void *)(__tmp_string_2540+19));
char *__tmp_string_2539="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_2539),(void *)(__tmp_string_2539+35));
char *__tmp_string_2538="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_2538),(void *)(__tmp_string_2538+30));
char *__tmp_string_2537="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_2537),(void *)(__tmp_string_2537+34));
char *__tmp_string_2536="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_2536),(void *)(__tmp_string_2536+36));
char *__tmp_string_2535="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_2535),(void *)(__tmp_string_2535+43));
char *__tmp_string_2534="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2534),(void *)(__tmp_string_2534+32));
char *__tmp_string_2533="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_2533),(void *)(__tmp_string_2533+33));
char *__tmp_string_2532="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_2532),(void *)(__tmp_string_2532+27));
char *__tmp_string_2531="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_2531),(void *)(__tmp_string_2531+37));
char *__tmp_string_2530="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_2530),(void *)(__tmp_string_2530+37));
char *__tmp_string_2529="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_2529),(void *)(__tmp_string_2529+28));
char *__tmp_string_2528="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_2528),(void *)(__tmp_string_2528+26));
char *__tmp_string_2527="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_2527),(void *)(__tmp_string_2527+32));
char *__tmp_string_2526="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_2526),(void *)(__tmp_string_2526+56));
char *__tmp_string_2525="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_2525),(void *)(__tmp_string_2525+46));
char *__tmp_string_2524="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_2524),(void *)(__tmp_string_2524+29));
char *__tmp_string_2523="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_2523),(void *)(__tmp_string_2523+39));
char *__tmp_string_2522="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_2522),(void *)(__tmp_string_2522+50));
char *__tmp_string_2521="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_2521),(void *)(__tmp_string_2521+31));
char *__tmp_string_2520="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_2520),(void *)(__tmp_string_2520+34));
char *__tmp_string_2519="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_2519),(void *)(__tmp_string_2519+25));
char *__tmp_string_2518="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_2518),(void *)(__tmp_string_2518+38));
char *__tmp_string_2517="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_2517),(void *)(__tmp_string_2517+15));
char *__tmp_string_2516="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_2516),(void *)(__tmp_string_2516+28));
char *__tmp_string_2515="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_2515),(void *)(__tmp_string_2515+29));
char *__tmp_string_2514="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_2514),(void *)(__tmp_string_2514+17));
char *__tmp_string_2513="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_2513),(void *)(__tmp_string_2513+35));
char *__tmp_string_2512="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_2512),(void *)(__tmp_string_2512+22));
char *__tmp_string_2511="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_2511),(void *)(__tmp_string_2511+35));
char *__tmp_string_2510="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_2510),(void *)(__tmp_string_2510+23));
char *__tmp_string_2509="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_2509),(void *)(__tmp_string_2509+29));
char *__tmp_string_2508="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_2508),(void *)(__tmp_string_2508+17));
char *__tmp_string_2507="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_2507),(void *)(__tmp_string_2507+31));
char *__tmp_string_2506="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_2506),(void *)(__tmp_string_2506+17));
char *__tmp_string_2505="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_2505),(void *)(__tmp_string_2505+33));
char *__tmp_string_2504="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2504),(void *)(__tmp_string_2504+36));
char *__tmp_string_2503="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_2503),(void *)(__tmp_string_2503+44));
char *__tmp_string_2502="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_2502),(void *)(__tmp_string_2502+40));
char *__tmp_string_2501="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_2501),(void *)(__tmp_string_2501+32));
char *__tmp_string_2500="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_2500),(void *)(__tmp_string_2500+32));
char *__tmp_string_2499="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_2499),(void *)(__tmp_string_2499+32));
char *__tmp_string_2498="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_2498),(void *)(__tmp_string_2498+26));
char *__tmp_string_2497="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_2497),(void *)(__tmp_string_2497+30));
char *__tmp_string_2496="the";
 __boundcheck_metadata_store((void *)(__tmp_string_2496),(void *)(__tmp_string_2496+3));
char *__tmp_string_2495="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_2495),(void *)(__tmp_string_2495+6));
char *__tmp_string_2494="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_2494),(void *)(__tmp_string_2494+21));
char *__tmp_string_2493="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_2493),(void *)(__tmp_string_2493+26));
char *__tmp_string_2492="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2492),(void *)(__tmp_string_2492+25));
char *__tmp_string_2491="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_2491),(void *)(__tmp_string_2491+19));
char *__tmp_string_2490="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_2490),(void *)(__tmp_string_2490+19));
char *__tmp_string_2489="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_2489),(void *)(__tmp_string_2489+38));
char *__tmp_string_2488="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2488),(void *)(__tmp_string_2488+23));
char *__tmp_string_2487="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_2487),(void *)(__tmp_string_2487+37));
char *__tmp_string_2486="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_2486),(void *)(__tmp_string_2486+38));
char *__tmp_string_2485="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_2485),(void *)(__tmp_string_2485+20));
char *__tmp_string_2484="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_2484),(void *)(__tmp_string_2484+52));
char *__tmp_string_2483="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_2483),(void *)(__tmp_string_2483+46));
char *__tmp_string_2482="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_2482),(void *)(__tmp_string_2482+17));
char *__tmp_string_2481="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_2481),(void *)(__tmp_string_2481+21));
char *__tmp_string_2480="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_2480),(void *)(__tmp_string_2480+32));
char *__tmp_string_2479="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_2479),(void *)(__tmp_string_2479+21));
char *__tmp_string_2478="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2478),(void *)(__tmp_string_2478+28));
char *__tmp_string_2477="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_2477),(void *)(__tmp_string_2477+20));
char *__tmp_string_2476="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_2476),(void *)(__tmp_string_2476+30));
char *__tmp_string_2475="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_2475),(void *)(__tmp_string_2475+16));
char *__tmp_string_2474="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_2474),(void *)(__tmp_string_2474+21));
char *__tmp_string_2473="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_2473),(void *)(__tmp_string_2473+34));
char *__tmp_string_2472="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_2472),(void *)(__tmp_string_2472+29));
char *__tmp_string_2471="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_2471),(void *)(__tmp_string_2471+41));
char *__tmp_string_2470="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_2470),(void *)(__tmp_string_2470+24));
char *__tmp_string_2469="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_2469),(void *)(__tmp_string_2469+23));
char *__tmp_string_2468="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_2468),(void *)(__tmp_string_2468+13));
char *__tmp_string_2467="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_2467),(void *)(__tmp_string_2467+33));
char *__tmp_string_2466="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_2466),(void *)(__tmp_string_2466+25));
char *__tmp_string_2465="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_2465),(void *)(__tmp_string_2465+41));
char *__tmp_string_2464="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_2464),(void *)(__tmp_string_2464+21));
char *__tmp_string_2463="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_2463),(void *)(__tmp_string_2463+23));
char *__tmp_string_2462="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_2462),(void *)(__tmp_string_2462+56));
char *__tmp_string_2461="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_2461),(void *)(__tmp_string_2461+37));
char *__tmp_string_2460="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_2460),(void *)(__tmp_string_2460+28));
char *__tmp_string_2459="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_2459),(void *)(__tmp_string_2459+26));
char *__tmp_string_2458="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_2458),(void *)(__tmp_string_2458+43));
char *__tmp_string_2457="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_2457),(void *)(__tmp_string_2457+27));
char *__tmp_string_2456="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_2456),(void *)(__tmp_string_2456+12));
char *__tmp_string_2455="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_2455),(void *)(__tmp_string_2455+12));
char *__tmp_string_2454="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_2454),(void *)(__tmp_string_2454+26));
char *__tmp_string_2453="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_2453),(void *)(__tmp_string_2453+16));
char *__tmp_string_2452="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_2452),(void *)(__tmp_string_2452+33));
char *__tmp_string_2451="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_2451),(void *)(__tmp_string_2451+20));
char *__tmp_string_2450="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_2450),(void *)(__tmp_string_2450+42));
char *__tmp_string_2449="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_2449),(void *)(__tmp_string_2449+38));
char *__tmp_string_2448="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_2448),(void *)(__tmp_string_2448+44));
char *__tmp_string_2447="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_2447),(void *)(__tmp_string_2447+13));
char *__tmp_string_2446="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_2446),(void *)(__tmp_string_2446+31));
char *__tmp_string_2445="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_2445),(void *)(__tmp_string_2445+14));
char *__tmp_string_2444="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_2444),(void *)(__tmp_string_2444+38));
char *__tmp_string_2443="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_2443),(void *)(__tmp_string_2443+26));
char *__tmp_string_2442="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_2442),(void *)(__tmp_string_2442+31));
char *__tmp_string_2441="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_2441),(void *)(__tmp_string_2441+15));
char *__tmp_string_2440="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2440),(void *)(__tmp_string_2440+26));
char *__tmp_string_2439="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_2439),(void *)(__tmp_string_2439+10));
char *__tmp_string_2438="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_2438),(void *)(__tmp_string_2438+8));
char *__tmp_string_2437="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_2437),(void *)(__tmp_string_2437+21));
char *__tmp_string_2436="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_2436),(void *)(__tmp_string_2436+37));
char *__tmp_string_2435="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_2435),(void *)(__tmp_string_2435+33));
char *__tmp_string_2434="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_2434),(void *)(__tmp_string_2434+27));
char *__tmp_string_2433="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_2433),(void *)(__tmp_string_2433+30));
char *__tmp_string_2432="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_2432),(void *)(__tmp_string_2432+27));
char *__tmp_string_2431="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_2431),(void *)(__tmp_string_2431+47));
char *__tmp_string_2430="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_2430),(void *)(__tmp_string_2430+22));
char *__tmp_string_2429="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_2429),(void *)(__tmp_string_2429+30));
char *__tmp_string_2428="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_2428),(void *)(__tmp_string_2428+29));
char *__tmp_string_2427="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_2427),(void *)(__tmp_string_2427+27));
char *__tmp_string_2426="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_2426),(void *)(__tmp_string_2426+29));
char *__tmp_string_2425="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_2425),(void *)(__tmp_string_2425+20));
char *__tmp_string_2424="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_2424),(void *)(__tmp_string_2424+26));
char *__tmp_string_2423="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_2423),(void *)(__tmp_string_2423+20));
char *__tmp_string_2422="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_2422),(void *)(__tmp_string_2422+27));
char *__tmp_string_2421="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_2421),(void *)(__tmp_string_2421+35));
char *__tmp_string_2420="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_2420),(void *)(__tmp_string_2420+34));
char *__tmp_string_2419="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_2419),(void *)(__tmp_string_2419+19));
char *__tmp_string_2418="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_2418),(void *)(__tmp_string_2418+35));
char *__tmp_string_2417="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_2417),(void *)(__tmp_string_2417+30));
char *__tmp_string_2416="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_2416),(void *)(__tmp_string_2416+34));
char *__tmp_string_2415="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_2415),(void *)(__tmp_string_2415+36));
char *__tmp_string_2414="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_2414),(void *)(__tmp_string_2414+43));
char *__tmp_string_2413="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2413),(void *)(__tmp_string_2413+32));
char *__tmp_string_2412="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_2412),(void *)(__tmp_string_2412+33));
char *__tmp_string_2411="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_2411),(void *)(__tmp_string_2411+27));
char *__tmp_string_2410="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_2410),(void *)(__tmp_string_2410+37));
char *__tmp_string_2409="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_2409),(void *)(__tmp_string_2409+37));
char *__tmp_string_2408="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_2408),(void *)(__tmp_string_2408+28));
char *__tmp_string_2407="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_2407),(void *)(__tmp_string_2407+26));
char *__tmp_string_2406="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_2406),(void *)(__tmp_string_2406+32));
char *__tmp_string_2405="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_2405),(void *)(__tmp_string_2405+56));
char *__tmp_string_2404="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_2404),(void *)(__tmp_string_2404+46));
char *__tmp_string_2403="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_2403),(void *)(__tmp_string_2403+29));
char *__tmp_string_2402="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_2402),(void *)(__tmp_string_2402+39));
char *__tmp_string_2401="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_2401),(void *)(__tmp_string_2401+50));
char *__tmp_string_2400="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_2400),(void *)(__tmp_string_2400+31));
char *__tmp_string_2399="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_2399),(void *)(__tmp_string_2399+34));
char *__tmp_string_2398="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_2398),(void *)(__tmp_string_2398+25));
char *__tmp_string_2397="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_2397),(void *)(__tmp_string_2397+38));
char *__tmp_string_2396="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_2396),(void *)(__tmp_string_2396+15));
char *__tmp_string_2395="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_2395),(void *)(__tmp_string_2395+28));
char *__tmp_string_2394="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_2394),(void *)(__tmp_string_2394+29));
char *__tmp_string_2393="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_2393),(void *)(__tmp_string_2393+17));
char *__tmp_string_2392="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_2392),(void *)(__tmp_string_2392+35));
char *__tmp_string_2391="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_2391),(void *)(__tmp_string_2391+22));
char *__tmp_string_2390="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_2390),(void *)(__tmp_string_2390+35));
char *__tmp_string_2389="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_2389),(void *)(__tmp_string_2389+23));
char *__tmp_string_2388="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_2388),(void *)(__tmp_string_2388+29));
char *__tmp_string_2387="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_2387),(void *)(__tmp_string_2387+17));
char *__tmp_string_2386="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_2386),(void *)(__tmp_string_2386+31));
char *__tmp_string_2385="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_2385),(void *)(__tmp_string_2385+17));
char *__tmp_string_2384="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_2384),(void *)(__tmp_string_2384+33));
char *__tmp_string_2383="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2383),(void *)(__tmp_string_2383+36));
char *__tmp_string_2382="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_2382),(void *)(__tmp_string_2382+44));
char *__tmp_string_2381="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_2381),(void *)(__tmp_string_2381+40));
char *__tmp_string_2380="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_2380),(void *)(__tmp_string_2380+32));
char *__tmp_string_2379="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_2379),(void *)(__tmp_string_2379+32));
char *__tmp_string_2378="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_2378),(void *)(__tmp_string_2378+32));
char *__tmp_string_2377="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_2377),(void *)(__tmp_string_2377+26));
char *__tmp_string_2376="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_2376),(void *)(__tmp_string_2376+30));
char *__tmp_string_2375="the";
 __boundcheck_metadata_store((void *)(__tmp_string_2375),(void *)(__tmp_string_2375+3));
char *__tmp_string_2374="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_2374),(void *)(__tmp_string_2374+6));
char *__tmp_string_2373="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_2373),(void *)(__tmp_string_2373+21));
char *__tmp_string_2372="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_2372),(void *)(__tmp_string_2372+26));
char *__tmp_string_2371="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2371),(void *)(__tmp_string_2371+25));
char *__tmp_string_2370="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_2370),(void *)(__tmp_string_2370+19));
char *__tmp_string_2369="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_2369),(void *)(__tmp_string_2369+19));
char *__tmp_string_2368="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_2368),(void *)(__tmp_string_2368+38));
char *__tmp_string_2367="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2367),(void *)(__tmp_string_2367+23));
char *__tmp_string_2366="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_2366),(void *)(__tmp_string_2366+37));
char *__tmp_string_2365="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_2365),(void *)(__tmp_string_2365+38));
char *__tmp_string_2364="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_2364),(void *)(__tmp_string_2364+20));
char *__tmp_string_2363="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_2363),(void *)(__tmp_string_2363+52));
char *__tmp_string_2362="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_2362),(void *)(__tmp_string_2362+46));
char *__tmp_string_2361="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_2361),(void *)(__tmp_string_2361+17));
char *__tmp_string_2360="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_2360),(void *)(__tmp_string_2360+21));
char *__tmp_string_2359="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_2359),(void *)(__tmp_string_2359+32));
char *__tmp_string_2358="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_2358),(void *)(__tmp_string_2358+21));
char *__tmp_string_2357="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2357),(void *)(__tmp_string_2357+28));
char *__tmp_string_2356="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_2356),(void *)(__tmp_string_2356+20));
char *__tmp_string_2355="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_2355),(void *)(__tmp_string_2355+30));
char *__tmp_string_2354="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_2354),(void *)(__tmp_string_2354+16));
char *__tmp_string_2353="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_2353),(void *)(__tmp_string_2353+21));
char *__tmp_string_2352="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_2352),(void *)(__tmp_string_2352+34));
char *__tmp_string_2351="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_2351),(void *)(__tmp_string_2351+29));
char *__tmp_string_2350="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_2350),(void *)(__tmp_string_2350+41));
char *__tmp_string_2349="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_2349),(void *)(__tmp_string_2349+24));
char *__tmp_string_2348="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_2348),(void *)(__tmp_string_2348+23));
char *__tmp_string_2347="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_2347),(void *)(__tmp_string_2347+13));
char *__tmp_string_2346="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_2346),(void *)(__tmp_string_2346+33));
char *__tmp_string_2345="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_2345),(void *)(__tmp_string_2345+25));
char *__tmp_string_2344="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_2344),(void *)(__tmp_string_2344+41));
char *__tmp_string_2343="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_2343),(void *)(__tmp_string_2343+21));
char *__tmp_string_2342="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_2342),(void *)(__tmp_string_2342+23));
char *__tmp_string_2341="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_2341),(void *)(__tmp_string_2341+56));
char *__tmp_string_2340="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_2340),(void *)(__tmp_string_2340+37));
char *__tmp_string_2339="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_2339),(void *)(__tmp_string_2339+28));
char *__tmp_string_2338="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_2338),(void *)(__tmp_string_2338+26));
char *__tmp_string_2337="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_2337),(void *)(__tmp_string_2337+43));
char *__tmp_string_2336="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_2336),(void *)(__tmp_string_2336+27));
char *__tmp_string_2335="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_2335),(void *)(__tmp_string_2335+12));
char *__tmp_string_2334="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_2334),(void *)(__tmp_string_2334+12));
char *__tmp_string_2333="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_2333),(void *)(__tmp_string_2333+26));
char *__tmp_string_2332="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_2332),(void *)(__tmp_string_2332+16));
char *__tmp_string_2331="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_2331),(void *)(__tmp_string_2331+33));
char *__tmp_string_2330="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_2330),(void *)(__tmp_string_2330+20));
char *__tmp_string_2329="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_2329),(void *)(__tmp_string_2329+42));
char *__tmp_string_2328="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_2328),(void *)(__tmp_string_2328+38));
char *__tmp_string_2327="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_2327),(void *)(__tmp_string_2327+44));
char *__tmp_string_2326="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_2326),(void *)(__tmp_string_2326+13));
char *__tmp_string_2325="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_2325),(void *)(__tmp_string_2325+31));
char *__tmp_string_2324="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_2324),(void *)(__tmp_string_2324+14));
char *__tmp_string_2323="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_2323),(void *)(__tmp_string_2323+38));
char *__tmp_string_2322="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_2322),(void *)(__tmp_string_2322+26));
char *__tmp_string_2321="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_2321),(void *)(__tmp_string_2321+31));
char *__tmp_string_2320="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_2320),(void *)(__tmp_string_2320+15));
char *__tmp_string_2319="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2319),(void *)(__tmp_string_2319+26));
char *__tmp_string_2318="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_2318),(void *)(__tmp_string_2318+10));
char *__tmp_string_2317="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_2317),(void *)(__tmp_string_2317+8));
char *__tmp_string_2316="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_2316),(void *)(__tmp_string_2316+21));
char *__tmp_string_2315="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_2315),(void *)(__tmp_string_2315+37));
char *__tmp_string_2314="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_2314),(void *)(__tmp_string_2314+33));
char *__tmp_string_2313="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_2313),(void *)(__tmp_string_2313+27));
char *__tmp_string_2312="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_2312),(void *)(__tmp_string_2312+30));
char *__tmp_string_2311="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_2311),(void *)(__tmp_string_2311+27));
char *__tmp_string_2310="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_2310),(void *)(__tmp_string_2310+47));
char *__tmp_string_2309="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_2309),(void *)(__tmp_string_2309+22));
char *__tmp_string_2308="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_2308),(void *)(__tmp_string_2308+30));
char *__tmp_string_2307="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_2307),(void *)(__tmp_string_2307+29));
char *__tmp_string_2306="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_2306),(void *)(__tmp_string_2306+27));
char *__tmp_string_2305="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_2305),(void *)(__tmp_string_2305+29));
char *__tmp_string_2304="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_2304),(void *)(__tmp_string_2304+20));
char *__tmp_string_2303="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_2303),(void *)(__tmp_string_2303+26));
char *__tmp_string_2302="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_2302),(void *)(__tmp_string_2302+20));
char *__tmp_string_2301="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_2301),(void *)(__tmp_string_2301+27));
char *__tmp_string_2300="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_2300),(void *)(__tmp_string_2300+35));
char *__tmp_string_2299="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_2299),(void *)(__tmp_string_2299+34));
char *__tmp_string_2298="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_2298),(void *)(__tmp_string_2298+19));
char *__tmp_string_2297="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_2297),(void *)(__tmp_string_2297+35));
char *__tmp_string_2296="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_2296),(void *)(__tmp_string_2296+30));
char *__tmp_string_2295="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_2295),(void *)(__tmp_string_2295+34));
char *__tmp_string_2294="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_2294),(void *)(__tmp_string_2294+36));
char *__tmp_string_2293="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_2293),(void *)(__tmp_string_2293+43));
char *__tmp_string_2292="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2292),(void *)(__tmp_string_2292+32));
char *__tmp_string_2291="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_2291),(void *)(__tmp_string_2291+33));
char *__tmp_string_2290="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_2290),(void *)(__tmp_string_2290+27));
char *__tmp_string_2289="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_2289),(void *)(__tmp_string_2289+37));
char *__tmp_string_2288="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_2288),(void *)(__tmp_string_2288+37));
char *__tmp_string_2287="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_2287),(void *)(__tmp_string_2287+28));
char *__tmp_string_2286="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_2286),(void *)(__tmp_string_2286+26));
char *__tmp_string_2285="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_2285),(void *)(__tmp_string_2285+32));
char *__tmp_string_2284="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_2284),(void *)(__tmp_string_2284+56));
char *__tmp_string_2283="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_2283),(void *)(__tmp_string_2283+46));
char *__tmp_string_2282="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_2282),(void *)(__tmp_string_2282+29));
char *__tmp_string_2281="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_2281),(void *)(__tmp_string_2281+39));
char *__tmp_string_2280="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_2280),(void *)(__tmp_string_2280+50));
char *__tmp_string_2279="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_2279),(void *)(__tmp_string_2279+31));
char *__tmp_string_2278="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_2278),(void *)(__tmp_string_2278+34));
char *__tmp_string_2277="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_2277),(void *)(__tmp_string_2277+25));
char *__tmp_string_2276="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_2276),(void *)(__tmp_string_2276+38));
char *__tmp_string_2275="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_2275),(void *)(__tmp_string_2275+15));
char *__tmp_string_2274="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_2274),(void *)(__tmp_string_2274+28));
char *__tmp_string_2273="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_2273),(void *)(__tmp_string_2273+29));
char *__tmp_string_2272="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_2272),(void *)(__tmp_string_2272+17));
char *__tmp_string_2271="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_2271),(void *)(__tmp_string_2271+35));
char *__tmp_string_2270="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_2270),(void *)(__tmp_string_2270+22));
char *__tmp_string_2269="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_2269),(void *)(__tmp_string_2269+35));
char *__tmp_string_2268="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_2268),(void *)(__tmp_string_2268+23));
char *__tmp_string_2267="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_2267),(void *)(__tmp_string_2267+29));
char *__tmp_string_2266="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_2266),(void *)(__tmp_string_2266+17));
char *__tmp_string_2265="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_2265),(void *)(__tmp_string_2265+31));
char *__tmp_string_2264="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_2264),(void *)(__tmp_string_2264+17));
char *__tmp_string_2263="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_2263),(void *)(__tmp_string_2263+33));
char *__tmp_string_2262="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2262),(void *)(__tmp_string_2262+36));
char *__tmp_string_2261="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_2261),(void *)(__tmp_string_2261+44));
char *__tmp_string_2260="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_2260),(void *)(__tmp_string_2260+40));
char *__tmp_string_2259="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_2259),(void *)(__tmp_string_2259+32));
char *__tmp_string_2258="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_2258),(void *)(__tmp_string_2258+32));
char *__tmp_string_2257="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_2257),(void *)(__tmp_string_2257+32));
char *__tmp_string_2256="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_2256),(void *)(__tmp_string_2256+26));
char *__tmp_string_2255="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_2255),(void *)(__tmp_string_2255+30));
char *__tmp_string_2254="the";
 __boundcheck_metadata_store((void *)(__tmp_string_2254),(void *)(__tmp_string_2254+3));
char *__tmp_string_2253="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_2253),(void *)(__tmp_string_2253+6));
char *__tmp_string_2252="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_2252),(void *)(__tmp_string_2252+21));
char *__tmp_string_2251="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_2251),(void *)(__tmp_string_2251+26));
char *__tmp_string_2250="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2250),(void *)(__tmp_string_2250+25));
char *__tmp_string_2249="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_2249),(void *)(__tmp_string_2249+19));
char *__tmp_string_2248="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_2248),(void *)(__tmp_string_2248+19));
char *__tmp_string_2247="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_2247),(void *)(__tmp_string_2247+38));
char *__tmp_string_2246="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2246),(void *)(__tmp_string_2246+23));
char *__tmp_string_2245="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_2245),(void *)(__tmp_string_2245+37));
char *__tmp_string_2244="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_2244),(void *)(__tmp_string_2244+38));
char *__tmp_string_2243="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_2243),(void *)(__tmp_string_2243+20));
char *__tmp_string_2242="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_2242),(void *)(__tmp_string_2242+52));
char *__tmp_string_2241="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_2241),(void *)(__tmp_string_2241+46));
char *__tmp_string_2240="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_2240),(void *)(__tmp_string_2240+17));
char *__tmp_string_2239="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_2239),(void *)(__tmp_string_2239+21));
char *__tmp_string_2238="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_2238),(void *)(__tmp_string_2238+32));
char *__tmp_string_2237="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_2237),(void *)(__tmp_string_2237+21));
char *__tmp_string_2236="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2236),(void *)(__tmp_string_2236+28));
char *__tmp_string_2235="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_2235),(void *)(__tmp_string_2235+20));
char *__tmp_string_2234="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_2234),(void *)(__tmp_string_2234+30));
char *__tmp_string_2233="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_2233),(void *)(__tmp_string_2233+16));
char *__tmp_string_2232="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_2232),(void *)(__tmp_string_2232+21));
char *__tmp_string_2231="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_2231),(void *)(__tmp_string_2231+34));
char *__tmp_string_2230="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_2230),(void *)(__tmp_string_2230+29));
char *__tmp_string_2229="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_2229),(void *)(__tmp_string_2229+41));
char *__tmp_string_2228="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_2228),(void *)(__tmp_string_2228+24));
char *__tmp_string_2227="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_2227),(void *)(__tmp_string_2227+23));
char *__tmp_string_2226="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_2226),(void *)(__tmp_string_2226+13));
char *__tmp_string_2225="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_2225),(void *)(__tmp_string_2225+33));
char *__tmp_string_2224="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_2224),(void *)(__tmp_string_2224+25));
char *__tmp_string_2223="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_2223),(void *)(__tmp_string_2223+41));
char *__tmp_string_2222="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_2222),(void *)(__tmp_string_2222+21));
char *__tmp_string_2221="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_2221),(void *)(__tmp_string_2221+23));
char *__tmp_string_2220="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_2220),(void *)(__tmp_string_2220+56));
char *__tmp_string_2219="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_2219),(void *)(__tmp_string_2219+37));
char *__tmp_string_2218="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_2218),(void *)(__tmp_string_2218+28));
char *__tmp_string_2217="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_2217),(void *)(__tmp_string_2217+26));
char *__tmp_string_2216="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_2216),(void *)(__tmp_string_2216+43));
char *__tmp_string_2215="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_2215),(void *)(__tmp_string_2215+27));
char *__tmp_string_2214="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_2214),(void *)(__tmp_string_2214+12));
char *__tmp_string_2213="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_2213),(void *)(__tmp_string_2213+12));
char *__tmp_string_2212="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_2212),(void *)(__tmp_string_2212+26));
char *__tmp_string_2211="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_2211),(void *)(__tmp_string_2211+16));
char *__tmp_string_2210="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_2210),(void *)(__tmp_string_2210+33));
char *__tmp_string_2209="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_2209),(void *)(__tmp_string_2209+20));
char *__tmp_string_2208="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_2208),(void *)(__tmp_string_2208+42));
char *__tmp_string_2207="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_2207),(void *)(__tmp_string_2207+38));
char *__tmp_string_2206="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_2206),(void *)(__tmp_string_2206+44));
char *__tmp_string_2205="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_2205),(void *)(__tmp_string_2205+13));
char *__tmp_string_2204="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_2204),(void *)(__tmp_string_2204+31));
char *__tmp_string_2203="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_2203),(void *)(__tmp_string_2203+14));
char *__tmp_string_2202="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_2202),(void *)(__tmp_string_2202+38));
char *__tmp_string_2201="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_2201),(void *)(__tmp_string_2201+26));
char *__tmp_string_2200="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_2200),(void *)(__tmp_string_2200+31));
char *__tmp_string_2199="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_2199),(void *)(__tmp_string_2199+15));
char *__tmp_string_2198="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2198),(void *)(__tmp_string_2198+26));
char *__tmp_string_2197="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_2197),(void *)(__tmp_string_2197+10));
char *__tmp_string_2196="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_2196),(void *)(__tmp_string_2196+8));
char *__tmp_string_2195="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_2195),(void *)(__tmp_string_2195+21));
char *__tmp_string_2194="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_2194),(void *)(__tmp_string_2194+37));
char *__tmp_string_2193="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_2193),(void *)(__tmp_string_2193+33));
char *__tmp_string_2192="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_2192),(void *)(__tmp_string_2192+27));
char *__tmp_string_2191="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_2191),(void *)(__tmp_string_2191+30));
char *__tmp_string_2190="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_2190),(void *)(__tmp_string_2190+27));
char *__tmp_string_2189="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_2189),(void *)(__tmp_string_2189+47));
char *__tmp_string_2188="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_2188),(void *)(__tmp_string_2188+22));
char *__tmp_string_2187="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_2187),(void *)(__tmp_string_2187+30));
char *__tmp_string_2186="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_2186),(void *)(__tmp_string_2186+29));
char *__tmp_string_2185="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_2185),(void *)(__tmp_string_2185+27));
char *__tmp_string_2184="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_2184),(void *)(__tmp_string_2184+29));
char *__tmp_string_2183="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_2183),(void *)(__tmp_string_2183+20));
char *__tmp_string_2182="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_2182),(void *)(__tmp_string_2182+26));
char *__tmp_string_2181="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_2181),(void *)(__tmp_string_2181+20));
char *__tmp_string_2180="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_2180),(void *)(__tmp_string_2180+27));
char *__tmp_string_2179="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_2179),(void *)(__tmp_string_2179+35));
char *__tmp_string_2178="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_2178),(void *)(__tmp_string_2178+34));
char *__tmp_string_2177="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_2177),(void *)(__tmp_string_2177+19));
char *__tmp_string_2176="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_2176),(void *)(__tmp_string_2176+35));
char *__tmp_string_2175="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_2175),(void *)(__tmp_string_2175+30));
char *__tmp_string_2174="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_2174),(void *)(__tmp_string_2174+34));
char *__tmp_string_2173="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_2173),(void *)(__tmp_string_2173+36));
char *__tmp_string_2172="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_2172),(void *)(__tmp_string_2172+43));
char *__tmp_string_2171="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2171),(void *)(__tmp_string_2171+32));
char *__tmp_string_2170="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_2170),(void *)(__tmp_string_2170+33));
char *__tmp_string_2169="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_2169),(void *)(__tmp_string_2169+27));
char *__tmp_string_2168="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_2168),(void *)(__tmp_string_2168+37));
char *__tmp_string_2167="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_2167),(void *)(__tmp_string_2167+37));
char *__tmp_string_2166="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_2166),(void *)(__tmp_string_2166+28));
char *__tmp_string_2165="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_2165),(void *)(__tmp_string_2165+26));
char *__tmp_string_2164="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_2164),(void *)(__tmp_string_2164+32));
char *__tmp_string_2163="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_2163),(void *)(__tmp_string_2163+56));
char *__tmp_string_2162="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_2162),(void *)(__tmp_string_2162+46));
char *__tmp_string_2161="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_2161),(void *)(__tmp_string_2161+29));
char *__tmp_string_2160="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_2160),(void *)(__tmp_string_2160+39));
char *__tmp_string_2159="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_2159),(void *)(__tmp_string_2159+50));
char *__tmp_string_2158="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_2158),(void *)(__tmp_string_2158+31));
char *__tmp_string_2157="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_2157),(void *)(__tmp_string_2157+34));
char *__tmp_string_2156="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_2156),(void *)(__tmp_string_2156+25));
char *__tmp_string_2155="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_2155),(void *)(__tmp_string_2155+38));
char *__tmp_string_2154="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_2154),(void *)(__tmp_string_2154+15));
char *__tmp_string_2153="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_2153),(void *)(__tmp_string_2153+28));
char *__tmp_string_2152="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_2152),(void *)(__tmp_string_2152+29));
char *__tmp_string_2151="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_2151),(void *)(__tmp_string_2151+17));
char *__tmp_string_2150="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_2150),(void *)(__tmp_string_2150+35));
char *__tmp_string_2149="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_2149),(void *)(__tmp_string_2149+22));
char *__tmp_string_2148="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_2148),(void *)(__tmp_string_2148+35));
char *__tmp_string_2147="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_2147),(void *)(__tmp_string_2147+23));
char *__tmp_string_2146="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_2146),(void *)(__tmp_string_2146+29));
char *__tmp_string_2145="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_2145),(void *)(__tmp_string_2145+17));
char *__tmp_string_2144="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_2144),(void *)(__tmp_string_2144+31));
char *__tmp_string_2143="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_2143),(void *)(__tmp_string_2143+17));
char *__tmp_string_2142="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_2142),(void *)(__tmp_string_2142+33));
char *__tmp_string_2141="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2141),(void *)(__tmp_string_2141+36));
char *__tmp_string_2140="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_2140),(void *)(__tmp_string_2140+44));
char *__tmp_string_2139="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_2139),(void *)(__tmp_string_2139+40));
char *__tmp_string_2138="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_2138),(void *)(__tmp_string_2138+32));
char *__tmp_string_2137="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_2137),(void *)(__tmp_string_2137+32));
char *__tmp_string_2136="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_2136),(void *)(__tmp_string_2136+32));
char *__tmp_string_2135="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_2135),(void *)(__tmp_string_2135+26));
char *__tmp_string_2134="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_2134),(void *)(__tmp_string_2134+30));
char *__tmp_string_2133="the";
 __boundcheck_metadata_store((void *)(__tmp_string_2133),(void *)(__tmp_string_2133+3));
char *__tmp_string_2132="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_2132),(void *)(__tmp_string_2132+6));
char *__tmp_string_2131="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_2131),(void *)(__tmp_string_2131+21));
char *__tmp_string_2130="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_2130),(void *)(__tmp_string_2130+26));
char *__tmp_string_2129="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2129),(void *)(__tmp_string_2129+25));
char *__tmp_string_2128="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_2128),(void *)(__tmp_string_2128+19));
char *__tmp_string_2127="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_2127),(void *)(__tmp_string_2127+19));
char *__tmp_string_2126="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_2126),(void *)(__tmp_string_2126+38));
char *__tmp_string_2125="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2125),(void *)(__tmp_string_2125+23));
char *__tmp_string_2124="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_2124),(void *)(__tmp_string_2124+37));
char *__tmp_string_2123="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_2123),(void *)(__tmp_string_2123+38));
char *__tmp_string_2122="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_2122),(void *)(__tmp_string_2122+20));
char *__tmp_string_2121="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_2121),(void *)(__tmp_string_2121+52));
char *__tmp_string_2120="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_2120),(void *)(__tmp_string_2120+46));
char *__tmp_string_2119="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_2119),(void *)(__tmp_string_2119+17));
char *__tmp_string_2118="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_2118),(void *)(__tmp_string_2118+21));
char *__tmp_string_2117="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_2117),(void *)(__tmp_string_2117+32));
char *__tmp_string_2116="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_2116),(void *)(__tmp_string_2116+21));
char *__tmp_string_2115="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2115),(void *)(__tmp_string_2115+28));
char *__tmp_string_2114="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_2114),(void *)(__tmp_string_2114+20));
char *__tmp_string_2113="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_2113),(void *)(__tmp_string_2113+30));
char *__tmp_string_2112="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_2112),(void *)(__tmp_string_2112+16));
char *__tmp_string_2111="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_2111),(void *)(__tmp_string_2111+21));
char *__tmp_string_2110="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_2110),(void *)(__tmp_string_2110+34));
char *__tmp_string_2109="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_2109),(void *)(__tmp_string_2109+29));
char *__tmp_string_2108="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_2108),(void *)(__tmp_string_2108+41));
char *__tmp_string_2107="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_2107),(void *)(__tmp_string_2107+24));
char *__tmp_string_2106="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_2106),(void *)(__tmp_string_2106+23));
char *__tmp_string_2105="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_2105),(void *)(__tmp_string_2105+13));
char *__tmp_string_2104="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_2104),(void *)(__tmp_string_2104+33));
char *__tmp_string_2103="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_2103),(void *)(__tmp_string_2103+25));
char *__tmp_string_2102="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_2102),(void *)(__tmp_string_2102+41));
char *__tmp_string_2101="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_2101),(void *)(__tmp_string_2101+21));
char *__tmp_string_2100="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_2100),(void *)(__tmp_string_2100+23));
char *__tmp_string_2099="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_2099),(void *)(__tmp_string_2099+56));
char *__tmp_string_2098="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_2098),(void *)(__tmp_string_2098+37));
char *__tmp_string_2097="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_2097),(void *)(__tmp_string_2097+28));
char *__tmp_string_2096="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_2096),(void *)(__tmp_string_2096+26));
char *__tmp_string_2095="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_2095),(void *)(__tmp_string_2095+43));
char *__tmp_string_2094="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_2094),(void *)(__tmp_string_2094+27));
char *__tmp_string_2093="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_2093),(void *)(__tmp_string_2093+12));
char *__tmp_string_2092="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_2092),(void *)(__tmp_string_2092+12));
char *__tmp_string_2091="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_2091),(void *)(__tmp_string_2091+26));
char *__tmp_string_2090="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_2090),(void *)(__tmp_string_2090+16));
char *__tmp_string_2089="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_2089),(void *)(__tmp_string_2089+33));
char *__tmp_string_2088="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_2088),(void *)(__tmp_string_2088+20));
char *__tmp_string_2087="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_2087),(void *)(__tmp_string_2087+42));
char *__tmp_string_2086="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_2086),(void *)(__tmp_string_2086+38));
char *__tmp_string_2085="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_2085),(void *)(__tmp_string_2085+44));
char *__tmp_string_2084="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_2084),(void *)(__tmp_string_2084+13));
char *__tmp_string_2083="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_2083),(void *)(__tmp_string_2083+31));
char *__tmp_string_2082="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_2082),(void *)(__tmp_string_2082+14));
char *__tmp_string_2081="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_2081),(void *)(__tmp_string_2081+38));
char *__tmp_string_2080="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_2080),(void *)(__tmp_string_2080+26));
char *__tmp_string_2079="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_2079),(void *)(__tmp_string_2079+31));
char *__tmp_string_2078="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_2078),(void *)(__tmp_string_2078+15));
char *__tmp_string_2077="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2077),(void *)(__tmp_string_2077+26));
char *__tmp_string_2076="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_2076),(void *)(__tmp_string_2076+10));
char *__tmp_string_2075="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_2075),(void *)(__tmp_string_2075+8));
char *__tmp_string_2074="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_2074),(void *)(__tmp_string_2074+21));
char *__tmp_string_2073="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_2073),(void *)(__tmp_string_2073+37));
char *__tmp_string_2072="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_2072),(void *)(__tmp_string_2072+33));
char *__tmp_string_2071="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_2071),(void *)(__tmp_string_2071+27));
char *__tmp_string_2070="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_2070),(void *)(__tmp_string_2070+30));
char *__tmp_string_2069="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_2069),(void *)(__tmp_string_2069+27));
char *__tmp_string_2068="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_2068),(void *)(__tmp_string_2068+47));
char *__tmp_string_2067="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_2067),(void *)(__tmp_string_2067+22));
char *__tmp_string_2066="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_2066),(void *)(__tmp_string_2066+30));
char *__tmp_string_2065="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_2065),(void *)(__tmp_string_2065+29));
char *__tmp_string_2064="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_2064),(void *)(__tmp_string_2064+27));
char *__tmp_string_2063="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_2063),(void *)(__tmp_string_2063+29));
char *__tmp_string_2062="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_2062),(void *)(__tmp_string_2062+20));
char *__tmp_string_2061="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_2061),(void *)(__tmp_string_2061+26));
char *__tmp_string_2060="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_2060),(void *)(__tmp_string_2060+20));
char *__tmp_string_2059="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_2059),(void *)(__tmp_string_2059+27));
char *__tmp_string_2058="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_2058),(void *)(__tmp_string_2058+35));
char *__tmp_string_2057="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_2057),(void *)(__tmp_string_2057+34));
char *__tmp_string_2056="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_2056),(void *)(__tmp_string_2056+19));
char *__tmp_string_2055="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_2055),(void *)(__tmp_string_2055+35));
char *__tmp_string_2054="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_2054),(void *)(__tmp_string_2054+30));
char *__tmp_string_2053="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_2053),(void *)(__tmp_string_2053+34));
char *__tmp_string_2052="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_2052),(void *)(__tmp_string_2052+36));
char *__tmp_string_2051="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_2051),(void *)(__tmp_string_2051+43));
char *__tmp_string_2050="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2050),(void *)(__tmp_string_2050+32));
char *__tmp_string_2049="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_2049),(void *)(__tmp_string_2049+33));
char *__tmp_string_2048="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_2048),(void *)(__tmp_string_2048+27));
char *__tmp_string_2047="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_2047),(void *)(__tmp_string_2047+37));
char *__tmp_string_2046="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_2046),(void *)(__tmp_string_2046+37));
char *__tmp_string_2045="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_2045),(void *)(__tmp_string_2045+28));
char *__tmp_string_2044="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_2044),(void *)(__tmp_string_2044+26));
char *__tmp_string_2043="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_2043),(void *)(__tmp_string_2043+32));
char *__tmp_string_2042="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_2042),(void *)(__tmp_string_2042+56));
char *__tmp_string_2041="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_2041),(void *)(__tmp_string_2041+46));
char *__tmp_string_2040="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_2040),(void *)(__tmp_string_2040+29));
char *__tmp_string_2039="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_2039),(void *)(__tmp_string_2039+39));
char *__tmp_string_2038="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_2038),(void *)(__tmp_string_2038+50));
char *__tmp_string_2037="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_2037),(void *)(__tmp_string_2037+31));
char *__tmp_string_2036="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_2036),(void *)(__tmp_string_2036+34));
char *__tmp_string_2035="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_2035),(void *)(__tmp_string_2035+25));
char *__tmp_string_2034="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_2034),(void *)(__tmp_string_2034+38));
char *__tmp_string_2033="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_2033),(void *)(__tmp_string_2033+15));
char *__tmp_string_2032="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_2032),(void *)(__tmp_string_2032+28));
char *__tmp_string_2031="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_2031),(void *)(__tmp_string_2031+29));
char *__tmp_string_2030="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_2030),(void *)(__tmp_string_2030+17));
char *__tmp_string_2029="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_2029),(void *)(__tmp_string_2029+35));
char *__tmp_string_2028="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_2028),(void *)(__tmp_string_2028+22));
char *__tmp_string_2027="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_2027),(void *)(__tmp_string_2027+35));
char *__tmp_string_2026="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_2026),(void *)(__tmp_string_2026+23));
char *__tmp_string_2025="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_2025),(void *)(__tmp_string_2025+29));
char *__tmp_string_2024="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_2024),(void *)(__tmp_string_2024+17));
char *__tmp_string_2023="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_2023),(void *)(__tmp_string_2023+31));
char *__tmp_string_2022="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_2022),(void *)(__tmp_string_2022+17));
char *__tmp_string_2021="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_2021),(void *)(__tmp_string_2021+33));
char *__tmp_string_2020="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_2020),(void *)(__tmp_string_2020+36));
char *__tmp_string_2019="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_2019),(void *)(__tmp_string_2019+44));
char *__tmp_string_2018="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_2018),(void *)(__tmp_string_2018+40));
char *__tmp_string_2017="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_2017),(void *)(__tmp_string_2017+32));
char *__tmp_string_2016="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_2016),(void *)(__tmp_string_2016+32));
char *__tmp_string_2015="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_2015),(void *)(__tmp_string_2015+32));
char *__tmp_string_2014="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_2014),(void *)(__tmp_string_2014+26));
char *__tmp_string_2013="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_2013),(void *)(__tmp_string_2013+30));
char *__tmp_string_2012="the";
 __boundcheck_metadata_store((void *)(__tmp_string_2012),(void *)(__tmp_string_2012+3));
char *__tmp_string_2011="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_2011),(void *)(__tmp_string_2011+6));
char *__tmp_string_2010="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_2010),(void *)(__tmp_string_2010+21));
char *__tmp_string_2009="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_2009),(void *)(__tmp_string_2009+26));
char *__tmp_string_2008="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_2008),(void *)(__tmp_string_2008+25));
char *__tmp_string_2007="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_2007),(void *)(__tmp_string_2007+19));
char *__tmp_string_2006="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_2006),(void *)(__tmp_string_2006+19));
char *__tmp_string_2005="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_2005),(void *)(__tmp_string_2005+38));
char *__tmp_string_2004="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_2004),(void *)(__tmp_string_2004+23));
char *__tmp_string_2003="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_2003),(void *)(__tmp_string_2003+37));
char *__tmp_string_2002="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_2002),(void *)(__tmp_string_2002+38));
char *__tmp_string_2001="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_2001),(void *)(__tmp_string_2001+20));
char *__tmp_string_2000="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_2000),(void *)(__tmp_string_2000+52));
char *__tmp_string_1999="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1999),(void *)(__tmp_string_1999+46));
char *__tmp_string_1998="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_1998),(void *)(__tmp_string_1998+17));
char *__tmp_string_1997="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_1997),(void *)(__tmp_string_1997+21));
char *__tmp_string_1996="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_1996),(void *)(__tmp_string_1996+32));
char *__tmp_string_1995="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_1995),(void *)(__tmp_string_1995+21));
char *__tmp_string_1994="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1994),(void *)(__tmp_string_1994+28));
char *__tmp_string_1993="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_1993),(void *)(__tmp_string_1993+20));
char *__tmp_string_1992="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_1992),(void *)(__tmp_string_1992+30));
char *__tmp_string_1991="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_1991),(void *)(__tmp_string_1991+16));
char *__tmp_string_1990="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_1990),(void *)(__tmp_string_1990+21));
char *__tmp_string_1989="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_1989),(void *)(__tmp_string_1989+34));
char *__tmp_string_1988="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_1988),(void *)(__tmp_string_1988+29));
char *__tmp_string_1987="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_1987),(void *)(__tmp_string_1987+41));
char *__tmp_string_1986="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_1986),(void *)(__tmp_string_1986+24));
char *__tmp_string_1985="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_1985),(void *)(__tmp_string_1985+23));
char *__tmp_string_1984="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_1984),(void *)(__tmp_string_1984+13));
char *__tmp_string_1983="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_1983),(void *)(__tmp_string_1983+33));
char *__tmp_string_1982="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_1982),(void *)(__tmp_string_1982+25));
char *__tmp_string_1981="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_1981),(void *)(__tmp_string_1981+41));
char *__tmp_string_1980="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_1980),(void *)(__tmp_string_1980+21));
char *__tmp_string_1979="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_1979),(void *)(__tmp_string_1979+23));
char *__tmp_string_1978="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_1978),(void *)(__tmp_string_1978+56));
char *__tmp_string_1977="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_1977),(void *)(__tmp_string_1977+37));
char *__tmp_string_1976="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_1976),(void *)(__tmp_string_1976+28));
char *__tmp_string_1975="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_1975),(void *)(__tmp_string_1975+26));
char *__tmp_string_1974="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_1974),(void *)(__tmp_string_1974+43));
char *__tmp_string_1973="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_1973),(void *)(__tmp_string_1973+27));
char *__tmp_string_1972="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_1972),(void *)(__tmp_string_1972+12));
char *__tmp_string_1971="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_1971),(void *)(__tmp_string_1971+12));
char *__tmp_string_1970="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_1970),(void *)(__tmp_string_1970+26));
char *__tmp_string_1969="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_1969),(void *)(__tmp_string_1969+16));
char *__tmp_string_1968="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_1968),(void *)(__tmp_string_1968+33));
char *__tmp_string_1967="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_1967),(void *)(__tmp_string_1967+20));
char *__tmp_string_1966="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_1966),(void *)(__tmp_string_1966+42));
char *__tmp_string_1965="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_1965),(void *)(__tmp_string_1965+38));
char *__tmp_string_1964="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_1964),(void *)(__tmp_string_1964+44));
char *__tmp_string_1963="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_1963),(void *)(__tmp_string_1963+13));
char *__tmp_string_1962="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_1962),(void *)(__tmp_string_1962+31));
char *__tmp_string_1961="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_1961),(void *)(__tmp_string_1961+14));
char *__tmp_string_1960="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_1960),(void *)(__tmp_string_1960+38));
char *__tmp_string_1959="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_1959),(void *)(__tmp_string_1959+26));
char *__tmp_string_1958="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_1958),(void *)(__tmp_string_1958+31));
char *__tmp_string_1957="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_1957),(void *)(__tmp_string_1957+15));
char *__tmp_string_1956="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1956),(void *)(__tmp_string_1956+26));
char *__tmp_string_1955="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_1955),(void *)(__tmp_string_1955+10));
char *__tmp_string_1954="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_1954),(void *)(__tmp_string_1954+8));
char *__tmp_string_1953="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_1953),(void *)(__tmp_string_1953+21));
char *__tmp_string_1952="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_1952),(void *)(__tmp_string_1952+37));
char *__tmp_string_1951="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_1951),(void *)(__tmp_string_1951+33));
char *__tmp_string_1950="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_1950),(void *)(__tmp_string_1950+27));
char *__tmp_string_1949="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1949),(void *)(__tmp_string_1949+30));
char *__tmp_string_1948="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_1948),(void *)(__tmp_string_1948+27));
char *__tmp_string_1947="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_1947),(void *)(__tmp_string_1947+47));
char *__tmp_string_1946="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_1946),(void *)(__tmp_string_1946+22));
char *__tmp_string_1945="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_1945),(void *)(__tmp_string_1945+30));
char *__tmp_string_1944="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_1944),(void *)(__tmp_string_1944+29));
char *__tmp_string_1943="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_1943),(void *)(__tmp_string_1943+27));
char *__tmp_string_1942="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_1942),(void *)(__tmp_string_1942+29));
char *__tmp_string_1941="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_1941),(void *)(__tmp_string_1941+20));
char *__tmp_string_1940="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_1940),(void *)(__tmp_string_1940+26));
char *__tmp_string_1939="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_1939),(void *)(__tmp_string_1939+20));
char *__tmp_string_1938="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_1938),(void *)(__tmp_string_1938+27));
char *__tmp_string_1937="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_1937),(void *)(__tmp_string_1937+35));
char *__tmp_string_1936="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_1936),(void *)(__tmp_string_1936+34));
char *__tmp_string_1935="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_1935),(void *)(__tmp_string_1935+19));
char *__tmp_string_1934="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_1934),(void *)(__tmp_string_1934+35));
char *__tmp_string_1933="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_1933),(void *)(__tmp_string_1933+30));
char *__tmp_string_1932="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_1932),(void *)(__tmp_string_1932+34));
char *__tmp_string_1931="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_1931),(void *)(__tmp_string_1931+36));
char *__tmp_string_1930="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_1930),(void *)(__tmp_string_1930+43));
char *__tmp_string_1929="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1929),(void *)(__tmp_string_1929+32));
char *__tmp_string_1928="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_1928),(void *)(__tmp_string_1928+33));
char *__tmp_string_1927="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_1927),(void *)(__tmp_string_1927+27));
char *__tmp_string_1926="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_1926),(void *)(__tmp_string_1926+37));
char *__tmp_string_1925="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_1925),(void *)(__tmp_string_1925+37));
char *__tmp_string_1924="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_1924),(void *)(__tmp_string_1924+28));
char *__tmp_string_1923="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_1923),(void *)(__tmp_string_1923+26));
char *__tmp_string_1922="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_1922),(void *)(__tmp_string_1922+32));
char *__tmp_string_1921="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_1921),(void *)(__tmp_string_1921+56));
char *__tmp_string_1920="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_1920),(void *)(__tmp_string_1920+46));
char *__tmp_string_1919="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_1919),(void *)(__tmp_string_1919+29));
char *__tmp_string_1918="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_1918),(void *)(__tmp_string_1918+39));
char *__tmp_string_1917="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_1917),(void *)(__tmp_string_1917+50));
char *__tmp_string_1916="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_1916),(void *)(__tmp_string_1916+31));
char *__tmp_string_1915="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_1915),(void *)(__tmp_string_1915+34));
char *__tmp_string_1914="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_1914),(void *)(__tmp_string_1914+25));
char *__tmp_string_1913="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_1913),(void *)(__tmp_string_1913+38));
char *__tmp_string_1912="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_1912),(void *)(__tmp_string_1912+15));
char *__tmp_string_1911="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_1911),(void *)(__tmp_string_1911+28));
char *__tmp_string_1910="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_1910),(void *)(__tmp_string_1910+29));
char *__tmp_string_1909="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_1909),(void *)(__tmp_string_1909+17));
char *__tmp_string_1908="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_1908),(void *)(__tmp_string_1908+35));
char *__tmp_string_1907="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_1907),(void *)(__tmp_string_1907+22));
char *__tmp_string_1906="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_1906),(void *)(__tmp_string_1906+35));
char *__tmp_string_1905="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_1905),(void *)(__tmp_string_1905+23));
char *__tmp_string_1904="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_1904),(void *)(__tmp_string_1904+29));
char *__tmp_string_1903="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_1903),(void *)(__tmp_string_1903+17));
char *__tmp_string_1902="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_1902),(void *)(__tmp_string_1902+31));
char *__tmp_string_1901="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_1901),(void *)(__tmp_string_1901+17));
char *__tmp_string_1900="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_1900),(void *)(__tmp_string_1900+33));
char *__tmp_string_1899="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1899),(void *)(__tmp_string_1899+36));
char *__tmp_string_1898="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_1898),(void *)(__tmp_string_1898+44));
char *__tmp_string_1897="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_1897),(void *)(__tmp_string_1897+40));
char *__tmp_string_1896="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_1896),(void *)(__tmp_string_1896+32));
char *__tmp_string_1895="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_1895),(void *)(__tmp_string_1895+32));
char *__tmp_string_1894="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_1894),(void *)(__tmp_string_1894+32));
char *__tmp_string_1893="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_1893),(void *)(__tmp_string_1893+26));
char *__tmp_string_1892="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_1892),(void *)(__tmp_string_1892+30));
char *__tmp_string_1891="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1891),(void *)(__tmp_string_1891+3));
char *__tmp_string_1890="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_1890),(void *)(__tmp_string_1890+6));
char *__tmp_string_1889="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_1889),(void *)(__tmp_string_1889+21));
char *__tmp_string_1888="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_1888),(void *)(__tmp_string_1888+26));
char *__tmp_string_1887="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1887),(void *)(__tmp_string_1887+25));
char *__tmp_string_1886="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_1886),(void *)(__tmp_string_1886+19));
char *__tmp_string_1885="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_1885),(void *)(__tmp_string_1885+19));
char *__tmp_string_1884="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_1884),(void *)(__tmp_string_1884+38));
char *__tmp_string_1883="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1883),(void *)(__tmp_string_1883+23));
char *__tmp_string_1882="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_1882),(void *)(__tmp_string_1882+37));
char *__tmp_string_1881="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_1881),(void *)(__tmp_string_1881+38));
char *__tmp_string_1880="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_1880),(void *)(__tmp_string_1880+20));
char *__tmp_string_1879="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_1879),(void *)(__tmp_string_1879+52));
char *__tmp_string_1878="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1878),(void *)(__tmp_string_1878+46));
char *__tmp_string_1877="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_1877),(void *)(__tmp_string_1877+17));
char *__tmp_string_1876="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_1876),(void *)(__tmp_string_1876+21));
char *__tmp_string_1875="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_1875),(void *)(__tmp_string_1875+32));
char *__tmp_string_1874="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_1874),(void *)(__tmp_string_1874+21));
char *__tmp_string_1873="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1873),(void *)(__tmp_string_1873+28));
char *__tmp_string_1872="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_1872),(void *)(__tmp_string_1872+20));
char *__tmp_string_1871="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_1871),(void *)(__tmp_string_1871+30));
char *__tmp_string_1870="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_1870),(void *)(__tmp_string_1870+16));
char *__tmp_string_1869="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_1869),(void *)(__tmp_string_1869+21));
char *__tmp_string_1868="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_1868),(void *)(__tmp_string_1868+34));
char *__tmp_string_1867="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_1867),(void *)(__tmp_string_1867+29));
char *__tmp_string_1866="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_1866),(void *)(__tmp_string_1866+41));
char *__tmp_string_1865="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_1865),(void *)(__tmp_string_1865+24));
char *__tmp_string_1864="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_1864),(void *)(__tmp_string_1864+23));
char *__tmp_string_1863="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_1863),(void *)(__tmp_string_1863+13));
char *__tmp_string_1862="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_1862),(void *)(__tmp_string_1862+33));
char *__tmp_string_1861="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_1861),(void *)(__tmp_string_1861+25));
char *__tmp_string_1860="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_1860),(void *)(__tmp_string_1860+41));
char *__tmp_string_1859="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_1859),(void *)(__tmp_string_1859+21));
char *__tmp_string_1858="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_1858),(void *)(__tmp_string_1858+23));
char *__tmp_string_1857="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_1857),(void *)(__tmp_string_1857+56));
char *__tmp_string_1856="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_1856),(void *)(__tmp_string_1856+37));
char *__tmp_string_1855="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_1855),(void *)(__tmp_string_1855+28));
char *__tmp_string_1854="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_1854),(void *)(__tmp_string_1854+26));
char *__tmp_string_1853="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_1853),(void *)(__tmp_string_1853+43));
char *__tmp_string_1852="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_1852),(void *)(__tmp_string_1852+27));
char *__tmp_string_1851="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_1851),(void *)(__tmp_string_1851+12));
char *__tmp_string_1850="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_1850),(void *)(__tmp_string_1850+12));
char *__tmp_string_1849="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_1849),(void *)(__tmp_string_1849+26));
char *__tmp_string_1848="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_1848),(void *)(__tmp_string_1848+16));
char *__tmp_string_1847="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_1847),(void *)(__tmp_string_1847+33));
char *__tmp_string_1846="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_1846),(void *)(__tmp_string_1846+20));
char *__tmp_string_1845="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_1845),(void *)(__tmp_string_1845+42));
char *__tmp_string_1844="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_1844),(void *)(__tmp_string_1844+38));
char *__tmp_string_1843="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_1843),(void *)(__tmp_string_1843+44));
char *__tmp_string_1842="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_1842),(void *)(__tmp_string_1842+13));
char *__tmp_string_1841="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_1841),(void *)(__tmp_string_1841+31));
char *__tmp_string_1840="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_1840),(void *)(__tmp_string_1840+14));
char *__tmp_string_1839="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_1839),(void *)(__tmp_string_1839+38));
char *__tmp_string_1838="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_1838),(void *)(__tmp_string_1838+26));
char *__tmp_string_1837="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_1837),(void *)(__tmp_string_1837+31));
char *__tmp_string_1836="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_1836),(void *)(__tmp_string_1836+15));
char *__tmp_string_1835="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1835),(void *)(__tmp_string_1835+26));
char *__tmp_string_1834="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_1834),(void *)(__tmp_string_1834+10));
char *__tmp_string_1833="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_1833),(void *)(__tmp_string_1833+8));
char *__tmp_string_1832="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_1832),(void *)(__tmp_string_1832+21));
char *__tmp_string_1831="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_1831),(void *)(__tmp_string_1831+37));
char *__tmp_string_1830="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_1830),(void *)(__tmp_string_1830+33));
char *__tmp_string_1829="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_1829),(void *)(__tmp_string_1829+27));
char *__tmp_string_1828="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1828),(void *)(__tmp_string_1828+30));
char *__tmp_string_1827="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_1827),(void *)(__tmp_string_1827+27));
char *__tmp_string_1826="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_1826),(void *)(__tmp_string_1826+47));
char *__tmp_string_1825="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_1825),(void *)(__tmp_string_1825+22));
char *__tmp_string_1824="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_1824),(void *)(__tmp_string_1824+30));
char *__tmp_string_1823="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_1823),(void *)(__tmp_string_1823+29));
char *__tmp_string_1822="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_1822),(void *)(__tmp_string_1822+27));
char *__tmp_string_1821="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_1821),(void *)(__tmp_string_1821+29));
char *__tmp_string_1820="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_1820),(void *)(__tmp_string_1820+20));
char *__tmp_string_1819="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_1819),(void *)(__tmp_string_1819+26));
char *__tmp_string_1818="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_1818),(void *)(__tmp_string_1818+20));
char *__tmp_string_1817="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_1817),(void *)(__tmp_string_1817+27));
char *__tmp_string_1816="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_1816),(void *)(__tmp_string_1816+35));
char *__tmp_string_1815="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_1815),(void *)(__tmp_string_1815+34));
char *__tmp_string_1814="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_1814),(void *)(__tmp_string_1814+19));
char *__tmp_string_1813="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_1813),(void *)(__tmp_string_1813+35));
char *__tmp_string_1812="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_1812),(void *)(__tmp_string_1812+30));
char *__tmp_string_1811="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_1811),(void *)(__tmp_string_1811+34));
char *__tmp_string_1810="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_1810),(void *)(__tmp_string_1810+36));
char *__tmp_string_1809="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_1809),(void *)(__tmp_string_1809+43));
char *__tmp_string_1808="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1808),(void *)(__tmp_string_1808+32));
char *__tmp_string_1807="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_1807),(void *)(__tmp_string_1807+33));
char *__tmp_string_1806="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_1806),(void *)(__tmp_string_1806+27));
char *__tmp_string_1805="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_1805),(void *)(__tmp_string_1805+37));
char *__tmp_string_1804="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_1804),(void *)(__tmp_string_1804+37));
char *__tmp_string_1803="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_1803),(void *)(__tmp_string_1803+28));
char *__tmp_string_1802="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_1802),(void *)(__tmp_string_1802+26));
char *__tmp_string_1801="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_1801),(void *)(__tmp_string_1801+32));
char *__tmp_string_1800="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_1800),(void *)(__tmp_string_1800+56));
char *__tmp_string_1799="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_1799),(void *)(__tmp_string_1799+46));
char *__tmp_string_1798="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_1798),(void *)(__tmp_string_1798+29));
char *__tmp_string_1797="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_1797),(void *)(__tmp_string_1797+39));
char *__tmp_string_1796="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_1796),(void *)(__tmp_string_1796+50));
char *__tmp_string_1795="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_1795),(void *)(__tmp_string_1795+31));
char *__tmp_string_1794="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_1794),(void *)(__tmp_string_1794+34));
char *__tmp_string_1793="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_1793),(void *)(__tmp_string_1793+25));
char *__tmp_string_1792="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_1792),(void *)(__tmp_string_1792+38));
char *__tmp_string_1791="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_1791),(void *)(__tmp_string_1791+15));
char *__tmp_string_1790="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_1790),(void *)(__tmp_string_1790+28));
char *__tmp_string_1789="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_1789),(void *)(__tmp_string_1789+29));
char *__tmp_string_1788="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_1788),(void *)(__tmp_string_1788+17));
char *__tmp_string_1787="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_1787),(void *)(__tmp_string_1787+35));
char *__tmp_string_1786="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_1786),(void *)(__tmp_string_1786+22));
char *__tmp_string_1785="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_1785),(void *)(__tmp_string_1785+35));
char *__tmp_string_1784="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_1784),(void *)(__tmp_string_1784+23));
char *__tmp_string_1783="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_1783),(void *)(__tmp_string_1783+29));
char *__tmp_string_1782="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_1782),(void *)(__tmp_string_1782+17));
char *__tmp_string_1781="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_1781),(void *)(__tmp_string_1781+31));
char *__tmp_string_1780="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_1780),(void *)(__tmp_string_1780+17));
char *__tmp_string_1779="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_1779),(void *)(__tmp_string_1779+33));
char *__tmp_string_1778="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1778),(void *)(__tmp_string_1778+36));
char *__tmp_string_1777="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_1777),(void *)(__tmp_string_1777+44));
char *__tmp_string_1776="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_1776),(void *)(__tmp_string_1776+40));
char *__tmp_string_1775="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_1775),(void *)(__tmp_string_1775+32));
char *__tmp_string_1774="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_1774),(void *)(__tmp_string_1774+32));
char *__tmp_string_1773="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_1773),(void *)(__tmp_string_1773+32));
char *__tmp_string_1772="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_1772),(void *)(__tmp_string_1772+26));
char *__tmp_string_1771="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_1771),(void *)(__tmp_string_1771+30));
char *__tmp_string_1770="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1770),(void *)(__tmp_string_1770+3));
char *__tmp_string_1769="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_1769),(void *)(__tmp_string_1769+6));
char *__tmp_string_1768="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_1768),(void *)(__tmp_string_1768+21));
char *__tmp_string_1767="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_1767),(void *)(__tmp_string_1767+26));
char *__tmp_string_1766="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1766),(void *)(__tmp_string_1766+25));
char *__tmp_string_1765="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_1765),(void *)(__tmp_string_1765+19));
char *__tmp_string_1764="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_1764),(void *)(__tmp_string_1764+19));
char *__tmp_string_1763="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_1763),(void *)(__tmp_string_1763+38));
char *__tmp_string_1762="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1762),(void *)(__tmp_string_1762+23));
char *__tmp_string_1761="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_1761),(void *)(__tmp_string_1761+37));
char *__tmp_string_1760="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_1760),(void *)(__tmp_string_1760+38));
char *__tmp_string_1759="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_1759),(void *)(__tmp_string_1759+20));
char *__tmp_string_1758="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_1758),(void *)(__tmp_string_1758+52));
char *__tmp_string_1757="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1757),(void *)(__tmp_string_1757+46));
char *__tmp_string_1756="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_1756),(void *)(__tmp_string_1756+17));
char *__tmp_string_1755="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_1755),(void *)(__tmp_string_1755+21));
char *__tmp_string_1754="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_1754),(void *)(__tmp_string_1754+32));
char *__tmp_string_1753="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_1753),(void *)(__tmp_string_1753+21));
char *__tmp_string_1752="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1752),(void *)(__tmp_string_1752+28));
char *__tmp_string_1751="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_1751),(void *)(__tmp_string_1751+20));
char *__tmp_string_1750="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_1750),(void *)(__tmp_string_1750+30));
char *__tmp_string_1749="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_1749),(void *)(__tmp_string_1749+16));
char *__tmp_string_1748="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_1748),(void *)(__tmp_string_1748+21));
char *__tmp_string_1747="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_1747),(void *)(__tmp_string_1747+34));
char *__tmp_string_1746="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_1746),(void *)(__tmp_string_1746+29));
char *__tmp_string_1745="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_1745),(void *)(__tmp_string_1745+41));
char *__tmp_string_1744="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_1744),(void *)(__tmp_string_1744+24));
char *__tmp_string_1743="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_1743),(void *)(__tmp_string_1743+23));
char *__tmp_string_1742="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_1742),(void *)(__tmp_string_1742+13));
char *__tmp_string_1741="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_1741),(void *)(__tmp_string_1741+33));
char *__tmp_string_1740="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_1740),(void *)(__tmp_string_1740+25));
char *__tmp_string_1739="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_1739),(void *)(__tmp_string_1739+41));
char *__tmp_string_1738="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_1738),(void *)(__tmp_string_1738+21));
char *__tmp_string_1737="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_1737),(void *)(__tmp_string_1737+23));
char *__tmp_string_1736="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_1736),(void *)(__tmp_string_1736+56));
char *__tmp_string_1735="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_1735),(void *)(__tmp_string_1735+37));
char *__tmp_string_1734="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_1734),(void *)(__tmp_string_1734+28));
char *__tmp_string_1733="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_1733),(void *)(__tmp_string_1733+26));
char *__tmp_string_1732="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_1732),(void *)(__tmp_string_1732+43));
char *__tmp_string_1731="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_1731),(void *)(__tmp_string_1731+27));
char *__tmp_string_1730="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_1730),(void *)(__tmp_string_1730+12));
char *__tmp_string_1729="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_1729),(void *)(__tmp_string_1729+12));
char *__tmp_string_1728="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_1728),(void *)(__tmp_string_1728+26));
char *__tmp_string_1727="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_1727),(void *)(__tmp_string_1727+16));
char *__tmp_string_1726="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_1726),(void *)(__tmp_string_1726+33));
char *__tmp_string_1725="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_1725),(void *)(__tmp_string_1725+20));
char *__tmp_string_1724="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_1724),(void *)(__tmp_string_1724+42));
char *__tmp_string_1723="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_1723),(void *)(__tmp_string_1723+38));
char *__tmp_string_1722="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_1722),(void *)(__tmp_string_1722+44));
char *__tmp_string_1721="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_1721),(void *)(__tmp_string_1721+13));
char *__tmp_string_1720="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_1720),(void *)(__tmp_string_1720+31));
char *__tmp_string_1719="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_1719),(void *)(__tmp_string_1719+14));
char *__tmp_string_1718="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_1718),(void *)(__tmp_string_1718+38));
char *__tmp_string_1717="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_1717),(void *)(__tmp_string_1717+26));
char *__tmp_string_1716="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_1716),(void *)(__tmp_string_1716+31));
char *__tmp_string_1715="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_1715),(void *)(__tmp_string_1715+15));
char *__tmp_string_1714="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1714),(void *)(__tmp_string_1714+26));
char *__tmp_string_1713="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_1713),(void *)(__tmp_string_1713+10));
char *__tmp_string_1712="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_1712),(void *)(__tmp_string_1712+8));
char *__tmp_string_1711="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_1711),(void *)(__tmp_string_1711+21));
char *__tmp_string_1710="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_1710),(void *)(__tmp_string_1710+37));
char *__tmp_string_1709="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_1709),(void *)(__tmp_string_1709+33));
char *__tmp_string_1708="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_1708),(void *)(__tmp_string_1708+27));
char *__tmp_string_1707="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1707),(void *)(__tmp_string_1707+30));
char *__tmp_string_1706="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_1706),(void *)(__tmp_string_1706+27));
char *__tmp_string_1705="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_1705),(void *)(__tmp_string_1705+47));
char *__tmp_string_1704="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_1704),(void *)(__tmp_string_1704+22));
char *__tmp_string_1703="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_1703),(void *)(__tmp_string_1703+30));
char *__tmp_string_1702="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_1702),(void *)(__tmp_string_1702+29));
char *__tmp_string_1701="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_1701),(void *)(__tmp_string_1701+27));
char *__tmp_string_1700="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_1700),(void *)(__tmp_string_1700+29));
char *__tmp_string_1699="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_1699),(void *)(__tmp_string_1699+20));
char *__tmp_string_1698="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_1698),(void *)(__tmp_string_1698+26));
char *__tmp_string_1697="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_1697),(void *)(__tmp_string_1697+20));
char *__tmp_string_1696="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_1696),(void *)(__tmp_string_1696+27));
char *__tmp_string_1695="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_1695),(void *)(__tmp_string_1695+35));
char *__tmp_string_1694="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_1694),(void *)(__tmp_string_1694+34));
char *__tmp_string_1693="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_1693),(void *)(__tmp_string_1693+19));
char *__tmp_string_1692="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_1692),(void *)(__tmp_string_1692+35));
char *__tmp_string_1691="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_1691),(void *)(__tmp_string_1691+30));
char *__tmp_string_1690="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_1690),(void *)(__tmp_string_1690+34));
char *__tmp_string_1689="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_1689),(void *)(__tmp_string_1689+36));
char *__tmp_string_1688="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_1688),(void *)(__tmp_string_1688+43));
char *__tmp_string_1687="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1687),(void *)(__tmp_string_1687+32));
char *__tmp_string_1686="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_1686),(void *)(__tmp_string_1686+33));
char *__tmp_string_1685="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_1685),(void *)(__tmp_string_1685+27));
char *__tmp_string_1684="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_1684),(void *)(__tmp_string_1684+37));
char *__tmp_string_1683="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_1683),(void *)(__tmp_string_1683+37));
char *__tmp_string_1682="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_1682),(void *)(__tmp_string_1682+28));
char *__tmp_string_1681="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_1681),(void *)(__tmp_string_1681+26));
char *__tmp_string_1680="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_1680),(void *)(__tmp_string_1680+32));
char *__tmp_string_1679="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_1679),(void *)(__tmp_string_1679+56));
char *__tmp_string_1678="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_1678),(void *)(__tmp_string_1678+46));
char *__tmp_string_1677="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_1677),(void *)(__tmp_string_1677+29));
char *__tmp_string_1676="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_1676),(void *)(__tmp_string_1676+39));
char *__tmp_string_1675="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_1675),(void *)(__tmp_string_1675+50));
char *__tmp_string_1674="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_1674),(void *)(__tmp_string_1674+31));
char *__tmp_string_1673="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_1673),(void *)(__tmp_string_1673+34));
char *__tmp_string_1672="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_1672),(void *)(__tmp_string_1672+25));
char *__tmp_string_1671="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_1671),(void *)(__tmp_string_1671+38));
char *__tmp_string_1670="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_1670),(void *)(__tmp_string_1670+15));
char *__tmp_string_1669="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_1669),(void *)(__tmp_string_1669+28));
char *__tmp_string_1668="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_1668),(void *)(__tmp_string_1668+29));
char *__tmp_string_1667="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_1667),(void *)(__tmp_string_1667+17));
char *__tmp_string_1666="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_1666),(void *)(__tmp_string_1666+35));
char *__tmp_string_1665="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_1665),(void *)(__tmp_string_1665+22));
char *__tmp_string_1664="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_1664),(void *)(__tmp_string_1664+35));
char *__tmp_string_1663="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_1663),(void *)(__tmp_string_1663+23));
char *__tmp_string_1662="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_1662),(void *)(__tmp_string_1662+29));
char *__tmp_string_1661="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_1661),(void *)(__tmp_string_1661+17));
char *__tmp_string_1660="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_1660),(void *)(__tmp_string_1660+31));
char *__tmp_string_1659="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_1659),(void *)(__tmp_string_1659+17));
char *__tmp_string_1658="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_1658),(void *)(__tmp_string_1658+33));
char *__tmp_string_1657="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1657),(void *)(__tmp_string_1657+36));
char *__tmp_string_1656="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_1656),(void *)(__tmp_string_1656+44));
char *__tmp_string_1655="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_1655),(void *)(__tmp_string_1655+40));
char *__tmp_string_1654="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_1654),(void *)(__tmp_string_1654+32));
char *__tmp_string_1653="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_1653),(void *)(__tmp_string_1653+32));
char *__tmp_string_1652="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_1652),(void *)(__tmp_string_1652+32));
char *__tmp_string_1651="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_1651),(void *)(__tmp_string_1651+26));
char *__tmp_string_1650="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_1650),(void *)(__tmp_string_1650+30));
char *__tmp_string_1649="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1649),(void *)(__tmp_string_1649+3));
char *__tmp_string_1648="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_1648),(void *)(__tmp_string_1648+6));
char *__tmp_string_1647="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_1647),(void *)(__tmp_string_1647+21));
char *__tmp_string_1646="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_1646),(void *)(__tmp_string_1646+26));
char *__tmp_string_1645="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1645),(void *)(__tmp_string_1645+25));
char *__tmp_string_1644="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_1644),(void *)(__tmp_string_1644+19));
char *__tmp_string_1643="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_1643),(void *)(__tmp_string_1643+19));
char *__tmp_string_1642="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_1642),(void *)(__tmp_string_1642+38));
char *__tmp_string_1641="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1641),(void *)(__tmp_string_1641+23));
char *__tmp_string_1640="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_1640),(void *)(__tmp_string_1640+37));
char *__tmp_string_1639="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_1639),(void *)(__tmp_string_1639+38));
char *__tmp_string_1638="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_1638),(void *)(__tmp_string_1638+20));
char *__tmp_string_1637="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_1637),(void *)(__tmp_string_1637+52));
char *__tmp_string_1636="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1636),(void *)(__tmp_string_1636+46));
char *__tmp_string_1635="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_1635),(void *)(__tmp_string_1635+17));
char *__tmp_string_1634="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_1634),(void *)(__tmp_string_1634+21));
char *__tmp_string_1633="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_1633),(void *)(__tmp_string_1633+32));
char *__tmp_string_1632="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_1632),(void *)(__tmp_string_1632+21));
char *__tmp_string_1631="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1631),(void *)(__tmp_string_1631+28));
char *__tmp_string_1630="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_1630),(void *)(__tmp_string_1630+20));
char *__tmp_string_1629="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_1629),(void *)(__tmp_string_1629+30));
char *__tmp_string_1628="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_1628),(void *)(__tmp_string_1628+16));
char *__tmp_string_1627="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_1627),(void *)(__tmp_string_1627+21));
char *__tmp_string_1626="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_1626),(void *)(__tmp_string_1626+34));
char *__tmp_string_1625="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_1625),(void *)(__tmp_string_1625+29));
char *__tmp_string_1624="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_1624),(void *)(__tmp_string_1624+41));
char *__tmp_string_1623="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_1623),(void *)(__tmp_string_1623+24));
char *__tmp_string_1622="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_1622),(void *)(__tmp_string_1622+23));
char *__tmp_string_1621="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_1621),(void *)(__tmp_string_1621+13));
char *__tmp_string_1620="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_1620),(void *)(__tmp_string_1620+33));
char *__tmp_string_1619="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_1619),(void *)(__tmp_string_1619+25));
char *__tmp_string_1618="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_1618),(void *)(__tmp_string_1618+41));
char *__tmp_string_1617="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_1617),(void *)(__tmp_string_1617+21));
char *__tmp_string_1616="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_1616),(void *)(__tmp_string_1616+23));
char *__tmp_string_1615="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_1615),(void *)(__tmp_string_1615+56));
char *__tmp_string_1614="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_1614),(void *)(__tmp_string_1614+37));
char *__tmp_string_1613="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_1613),(void *)(__tmp_string_1613+28));
char *__tmp_string_1612="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_1612),(void *)(__tmp_string_1612+26));
char *__tmp_string_1611="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_1611),(void *)(__tmp_string_1611+43));
char *__tmp_string_1610="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_1610),(void *)(__tmp_string_1610+27));
char *__tmp_string_1609="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_1609),(void *)(__tmp_string_1609+12));
char *__tmp_string_1608="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_1608),(void *)(__tmp_string_1608+12));
char *__tmp_string_1607="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_1607),(void *)(__tmp_string_1607+26));
char *__tmp_string_1606="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_1606),(void *)(__tmp_string_1606+16));
char *__tmp_string_1605="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_1605),(void *)(__tmp_string_1605+33));
char *__tmp_string_1604="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_1604),(void *)(__tmp_string_1604+20));
char *__tmp_string_1603="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_1603),(void *)(__tmp_string_1603+42));
char *__tmp_string_1602="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_1602),(void *)(__tmp_string_1602+38));
char *__tmp_string_1601="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_1601),(void *)(__tmp_string_1601+44));
char *__tmp_string_1600="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_1600),(void *)(__tmp_string_1600+13));
char *__tmp_string_1599="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_1599),(void *)(__tmp_string_1599+31));
char *__tmp_string_1598="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_1598),(void *)(__tmp_string_1598+14));
char *__tmp_string_1597="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_1597),(void *)(__tmp_string_1597+38));
char *__tmp_string_1596="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_1596),(void *)(__tmp_string_1596+26));
char *__tmp_string_1595="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_1595),(void *)(__tmp_string_1595+31));
char *__tmp_string_1594="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_1594),(void *)(__tmp_string_1594+15));
char *__tmp_string_1593="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1593),(void *)(__tmp_string_1593+26));
char *__tmp_string_1592="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_1592),(void *)(__tmp_string_1592+10));
char *__tmp_string_1591="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_1591),(void *)(__tmp_string_1591+8));
char *__tmp_string_1590="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_1590),(void *)(__tmp_string_1590+21));
char *__tmp_string_1589="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_1589),(void *)(__tmp_string_1589+37));
char *__tmp_string_1588="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_1588),(void *)(__tmp_string_1588+33));
char *__tmp_string_1587="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_1587),(void *)(__tmp_string_1587+27));
char *__tmp_string_1586="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1586),(void *)(__tmp_string_1586+30));
char *__tmp_string_1585="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_1585),(void *)(__tmp_string_1585+27));
char *__tmp_string_1584="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_1584),(void *)(__tmp_string_1584+47));
char *__tmp_string_1583="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_1583),(void *)(__tmp_string_1583+22));
char *__tmp_string_1582="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_1582),(void *)(__tmp_string_1582+30));
char *__tmp_string_1581="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_1581),(void *)(__tmp_string_1581+29));
char *__tmp_string_1580="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_1580),(void *)(__tmp_string_1580+27));
char *__tmp_string_1579="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_1579),(void *)(__tmp_string_1579+29));
char *__tmp_string_1578="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_1578),(void *)(__tmp_string_1578+20));
char *__tmp_string_1577="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_1577),(void *)(__tmp_string_1577+26));
char *__tmp_string_1576="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_1576),(void *)(__tmp_string_1576+20));
char *__tmp_string_1575="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_1575),(void *)(__tmp_string_1575+27));
char *__tmp_string_1574="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_1574),(void *)(__tmp_string_1574+35));
char *__tmp_string_1573="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_1573),(void *)(__tmp_string_1573+34));
char *__tmp_string_1572="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_1572),(void *)(__tmp_string_1572+19));
char *__tmp_string_1571="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_1571),(void *)(__tmp_string_1571+35));
char *__tmp_string_1570="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_1570),(void *)(__tmp_string_1570+30));
char *__tmp_string_1569="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_1569),(void *)(__tmp_string_1569+34));
char *__tmp_string_1568="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_1568),(void *)(__tmp_string_1568+36));
char *__tmp_string_1567="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_1567),(void *)(__tmp_string_1567+43));
char *__tmp_string_1566="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1566),(void *)(__tmp_string_1566+32));
char *__tmp_string_1565="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_1565),(void *)(__tmp_string_1565+33));
char *__tmp_string_1564="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_1564),(void *)(__tmp_string_1564+27));
char *__tmp_string_1563="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_1563),(void *)(__tmp_string_1563+37));
char *__tmp_string_1562="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_1562),(void *)(__tmp_string_1562+37));
char *__tmp_string_1561="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_1561),(void *)(__tmp_string_1561+28));
char *__tmp_string_1560="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_1560),(void *)(__tmp_string_1560+26));
char *__tmp_string_1559="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_1559),(void *)(__tmp_string_1559+32));
char *__tmp_string_1558="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_1558),(void *)(__tmp_string_1558+56));
char *__tmp_string_1557="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_1557),(void *)(__tmp_string_1557+46));
char *__tmp_string_1556="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_1556),(void *)(__tmp_string_1556+29));
char *__tmp_string_1555="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_1555),(void *)(__tmp_string_1555+39));
char *__tmp_string_1554="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_1554),(void *)(__tmp_string_1554+50));
char *__tmp_string_1553="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_1553),(void *)(__tmp_string_1553+31));
char *__tmp_string_1552="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_1552),(void *)(__tmp_string_1552+34));
char *__tmp_string_1551="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_1551),(void *)(__tmp_string_1551+25));
char *__tmp_string_1550="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_1550),(void *)(__tmp_string_1550+38));
char *__tmp_string_1549="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_1549),(void *)(__tmp_string_1549+15));
char *__tmp_string_1548="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_1548),(void *)(__tmp_string_1548+28));
char *__tmp_string_1547="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_1547),(void *)(__tmp_string_1547+29));
char *__tmp_string_1546="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_1546),(void *)(__tmp_string_1546+17));
char *__tmp_string_1545="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_1545),(void *)(__tmp_string_1545+35));
char *__tmp_string_1544="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_1544),(void *)(__tmp_string_1544+22));
char *__tmp_string_1543="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_1543),(void *)(__tmp_string_1543+35));
char *__tmp_string_1542="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_1542),(void *)(__tmp_string_1542+23));
char *__tmp_string_1541="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_1541),(void *)(__tmp_string_1541+29));
char *__tmp_string_1540="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_1540),(void *)(__tmp_string_1540+17));
char *__tmp_string_1539="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_1539),(void *)(__tmp_string_1539+31));
char *__tmp_string_1538="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_1538),(void *)(__tmp_string_1538+17));
char *__tmp_string_1537="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_1537),(void *)(__tmp_string_1537+33));
char *__tmp_string_1536="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1536),(void *)(__tmp_string_1536+36));
char *__tmp_string_1535="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_1535),(void *)(__tmp_string_1535+44));
char *__tmp_string_1534="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_1534),(void *)(__tmp_string_1534+40));
char *__tmp_string_1533="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_1533),(void *)(__tmp_string_1533+32));
char *__tmp_string_1532="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_1532),(void *)(__tmp_string_1532+32));
char *__tmp_string_1531="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_1531),(void *)(__tmp_string_1531+32));
char *__tmp_string_1530="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_1530),(void *)(__tmp_string_1530+26));
char *__tmp_string_1529="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_1529),(void *)(__tmp_string_1529+30));
char *__tmp_string_1528="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1528),(void *)(__tmp_string_1528+3));
char *__tmp_string_1527="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_1527),(void *)(__tmp_string_1527+6));
char *__tmp_string_1526="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_1526),(void *)(__tmp_string_1526+21));
char *__tmp_string_1525="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_1525),(void *)(__tmp_string_1525+26));
char *__tmp_string_1524="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1524),(void *)(__tmp_string_1524+25));
char *__tmp_string_1523="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_1523),(void *)(__tmp_string_1523+19));
char *__tmp_string_1522="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_1522),(void *)(__tmp_string_1522+19));
char *__tmp_string_1521="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_1521),(void *)(__tmp_string_1521+38));
char *__tmp_string_1520="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1520),(void *)(__tmp_string_1520+23));
char *__tmp_string_1519="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_1519),(void *)(__tmp_string_1519+37));
char *__tmp_string_1518="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_1518),(void *)(__tmp_string_1518+38));
char *__tmp_string_1517="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_1517),(void *)(__tmp_string_1517+20));
char *__tmp_string_1516="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_1516),(void *)(__tmp_string_1516+52));
char *__tmp_string_1515="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1515),(void *)(__tmp_string_1515+46));
char *__tmp_string_1514="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_1514),(void *)(__tmp_string_1514+17));
char *__tmp_string_1513="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_1513),(void *)(__tmp_string_1513+21));
char *__tmp_string_1512="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_1512),(void *)(__tmp_string_1512+32));
char *__tmp_string_1511="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_1511),(void *)(__tmp_string_1511+21));
char *__tmp_string_1510="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1510),(void *)(__tmp_string_1510+28));
char *__tmp_string_1509="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_1509),(void *)(__tmp_string_1509+20));
char *__tmp_string_1508="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_1508),(void *)(__tmp_string_1508+30));
char *__tmp_string_1507="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_1507),(void *)(__tmp_string_1507+16));
char *__tmp_string_1506="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_1506),(void *)(__tmp_string_1506+21));
char *__tmp_string_1505="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_1505),(void *)(__tmp_string_1505+34));
char *__tmp_string_1504="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_1504),(void *)(__tmp_string_1504+29));
char *__tmp_string_1503="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_1503),(void *)(__tmp_string_1503+41));
char *__tmp_string_1502="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_1502),(void *)(__tmp_string_1502+24));
char *__tmp_string_1501="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_1501),(void *)(__tmp_string_1501+23));
char *__tmp_string_1500="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_1500),(void *)(__tmp_string_1500+13));
char *__tmp_string_1499="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_1499),(void *)(__tmp_string_1499+33));
char *__tmp_string_1498="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_1498),(void *)(__tmp_string_1498+25));
char *__tmp_string_1497="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_1497),(void *)(__tmp_string_1497+41));
char *__tmp_string_1496="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_1496),(void *)(__tmp_string_1496+21));
char *__tmp_string_1495="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_1495),(void *)(__tmp_string_1495+23));
char *__tmp_string_1494="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_1494),(void *)(__tmp_string_1494+56));
char *__tmp_string_1493="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_1493),(void *)(__tmp_string_1493+37));
char *__tmp_string_1492="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_1492),(void *)(__tmp_string_1492+28));
char *__tmp_string_1491="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_1491),(void *)(__tmp_string_1491+26));
char *__tmp_string_1490="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_1490),(void *)(__tmp_string_1490+43));
char *__tmp_string_1489="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_1489),(void *)(__tmp_string_1489+27));
char *__tmp_string_1488="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_1488),(void *)(__tmp_string_1488+12));
char *__tmp_string_1487="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_1487),(void *)(__tmp_string_1487+12));
char *__tmp_string_1486="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_1486),(void *)(__tmp_string_1486+26));
char *__tmp_string_1485="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_1485),(void *)(__tmp_string_1485+16));
char *__tmp_string_1484="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_1484),(void *)(__tmp_string_1484+33));
char *__tmp_string_1483="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_1483),(void *)(__tmp_string_1483+20));
char *__tmp_string_1482="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_1482),(void *)(__tmp_string_1482+42));
char *__tmp_string_1481="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_1481),(void *)(__tmp_string_1481+38));
char *__tmp_string_1480="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_1480),(void *)(__tmp_string_1480+44));
char *__tmp_string_1479="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_1479),(void *)(__tmp_string_1479+13));
char *__tmp_string_1478="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_1478),(void *)(__tmp_string_1478+31));
char *__tmp_string_1477="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_1477),(void *)(__tmp_string_1477+14));
char *__tmp_string_1476="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_1476),(void *)(__tmp_string_1476+38));
char *__tmp_string_1475="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_1475),(void *)(__tmp_string_1475+26));
char *__tmp_string_1474="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_1474),(void *)(__tmp_string_1474+31));
char *__tmp_string_1473="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_1473),(void *)(__tmp_string_1473+15));
char *__tmp_string_1472="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1472),(void *)(__tmp_string_1472+26));
char *__tmp_string_1471="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_1471),(void *)(__tmp_string_1471+10));
char *__tmp_string_1470="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_1470),(void *)(__tmp_string_1470+8));
char *__tmp_string_1469="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_1469),(void *)(__tmp_string_1469+21));
char *__tmp_string_1468="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_1468),(void *)(__tmp_string_1468+37));
char *__tmp_string_1467="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_1467),(void *)(__tmp_string_1467+33));
char *__tmp_string_1466="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_1466),(void *)(__tmp_string_1466+27));
char *__tmp_string_1465="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1465),(void *)(__tmp_string_1465+30));
char *__tmp_string_1464="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_1464),(void *)(__tmp_string_1464+27));
char *__tmp_string_1463="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_1463),(void *)(__tmp_string_1463+47));
char *__tmp_string_1462="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_1462),(void *)(__tmp_string_1462+22));
char *__tmp_string_1461="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_1461),(void *)(__tmp_string_1461+30));
char *__tmp_string_1460="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_1460),(void *)(__tmp_string_1460+29));
char *__tmp_string_1459="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_1459),(void *)(__tmp_string_1459+27));
char *__tmp_string_1458="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_1458),(void *)(__tmp_string_1458+29));
char *__tmp_string_1457="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_1457),(void *)(__tmp_string_1457+20));
char *__tmp_string_1456="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_1456),(void *)(__tmp_string_1456+26));
char *__tmp_string_1455="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_1455),(void *)(__tmp_string_1455+20));
char *__tmp_string_1454="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_1454),(void *)(__tmp_string_1454+27));
char *__tmp_string_1453="worth But trust me on the sunscreen";
 __boundcheck_metadata_store((void *)(__tmp_string_1453),(void *)(__tmp_string_1453+35));
char *__tmp_string_1452="and recycling it for more than its";
 __boundcheck_metadata_store((void *)(__tmp_string_1452),(void *)(__tmp_string_1452+34));
char *__tmp_string_1451="over the ugly parts";
 __boundcheck_metadata_store((void *)(__tmp_string_1451),(void *)(__tmp_string_1451+19));
char *__tmp_string_1450="the disposal wiping it off painting";
 __boundcheck_metadata_store((void *)(__tmp_string_1450),(void *)(__tmp_string_1450+35));
char *__tmp_string_1449="a way of fishing the past from";
 __boundcheck_metadata_store((void *)(__tmp_string_1449),(void *)(__tmp_string_1449+30));
char *__tmp_string_1448="form of nostalgia Dispensing it is";
 __boundcheck_metadata_store((void *)(__tmp_string_1448),(void *)(__tmp_string_1448+34));
char *__tmp_string_1447="with those who supply it Advice is a";
 __boundcheck_metadata_store((void *)(__tmp_string_1447),(void *)(__tmp_string_1447+36));
char *__tmp_string_1446="careful whose advice you buy but be patient";
 __boundcheck_metadata_store((void *)(__tmp_string_1446),(void *)(__tmp_string_1446+43));
char *__tmp_string_1445="time youre 40 it will look 85 Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1445),(void *)(__tmp_string_1445+32));
char *__tmp_string_1444="too much with your hair or by the";
 __boundcheck_metadata_store((void *)(__tmp_string_1444),(void *)(__tmp_string_1444+33));
char *__tmp_string_1443="one might run out Dont mess";
 __boundcheck_metadata_store((void *)(__tmp_string_1443),(void *)(__tmp_string_1443+27));
char *__tmp_string_1442="spouse But you never know when either";
 __boundcheck_metadata_store((void *)(__tmp_string_1442),(void *)(__tmp_string_1442+37));
char *__tmp_string_1441="trust fund Maybe youll have a wealthy";
 __boundcheck_metadata_store((void *)(__tmp_string_1441),(void *)(__tmp_string_1441+37));
char *__tmp_string_1440="support you Maybe you have a";
 __boundcheck_metadata_store((void *)(__tmp_string_1440),(void *)(__tmp_string_1440+28));
char *__tmp_string_1439="Dont expect anyone else to";
 __boundcheck_metadata_store((void *)(__tmp_string_1439),(void *)(__tmp_string_1439+26));
char *__tmp_string_1438="their elders Respect your elders";
 __boundcheck_metadata_store((void *)(__tmp_string_1438),(void *)(__tmp_string_1438+32));
char *__tmp_string_1437="reasonable politicians were noble and children respected";
 __boundcheck_metadata_store((void *)(__tmp_string_1437),(void *)(__tmp_string_1437+56));
char *__tmp_string_1436="fantasize that when you were young prices were";
 __boundcheck_metadata_store((void *)(__tmp_string_1436),(void *)(__tmp_string_1436+46));
char *__tmp_string_1435="get old And when you do youll";
 __boundcheck_metadata_store((void *)(__tmp_string_1435),(void *)(__tmp_string_1435+29));
char *__tmp_string_1434="Politicians will philander You too will";
 __boundcheck_metadata_store((void *)(__tmp_string_1434),(void *)(__tmp_string_1434+39));
char *__tmp_string_1433="Accept certain inalienable truths Prices will rise";
 __boundcheck_metadata_store((void *)(__tmp_string_1433),(void *)(__tmp_string_1433+50));
char *__tmp_string_1432="before it makes you soft Travel";
 __boundcheck_metadata_store((void *)(__tmp_string_1432),(void *)(__tmp_string_1432+31));
char *__tmp_string_1431="Northern California once but leave";
 __boundcheck_metadata_store((void *)(__tmp_string_1431),(void *)(__tmp_string_1431+34));
char *__tmp_string_1430="it makes you hard Live in";
 __boundcheck_metadata_store((void *)(__tmp_string_1430),(void *)(__tmp_string_1430+25));
char *__tmp_string_1429="in New York City once but leave before";
 __boundcheck_metadata_store((void *)(__tmp_string_1429),(void *)(__tmp_string_1429+38));
char *__tmp_string_1428="were young Live";
 __boundcheck_metadata_store((void *)(__tmp_string_1428),(void *)(__tmp_string_1428+15));
char *__tmp_string_1427="people who knew you when you";
 __boundcheck_metadata_store((void *)(__tmp_string_1427),(void *)(__tmp_string_1427+28));
char *__tmp_string_1426="you get the more you need the";
 __boundcheck_metadata_store((void *)(__tmp_string_1426),(void *)(__tmp_string_1426+29));
char *__tmp_string_1425="because the older";
 __boundcheck_metadata_store((void *)(__tmp_string_1425),(void *)(__tmp_string_1425+17));
char *__tmp_string_1424="the gaps in geography and lifestyle";
 __boundcheck_metadata_store((void *)(__tmp_string_1424),(void *)(__tmp_string_1424+35));
char *__tmp_string_1423="on Work hard to bridge";
 __boundcheck_metadata_store((void *)(__tmp_string_1423),(void *)(__tmp_string_1423+22));
char *__tmp_string_1422="with a precious few you should hold";
 __boundcheck_metadata_store((void *)(__tmp_string_1422),(void *)(__tmp_string_1422+35));
char *__tmp_string_1421="friends come and go but";
 __boundcheck_metadata_store((void *)(__tmp_string_1421),(void *)(__tmp_string_1421+23));
char *__tmp_string_1420="in the future Understand that";
 __boundcheck_metadata_store((void *)(__tmp_string_1420),(void *)(__tmp_string_1420+29));
char *__tmp_string_1419="to stick with you";
 __boundcheck_metadata_store((void *)(__tmp_string_1419),(void *)(__tmp_string_1419+17));
char *__tmp_string_1418="past and the people most likely";
 __boundcheck_metadata_store((void *)(__tmp_string_1418),(void *)(__tmp_string_1418+31));
char *__tmp_string_1417="best link to your";
 __boundcheck_metadata_store((void *)(__tmp_string_1417),(void *)(__tmp_string_1417+17));
char *__tmp_string_1416="nice to your siblings Theyre your";
 __boundcheck_metadata_store((void *)(__tmp_string_1416),(void *)(__tmp_string_1416+33));
char *__tmp_string_1415="know when theyll be gone for good Be";
 __boundcheck_metadata_store((void *)(__tmp_string_1415),(void *)(__tmp_string_1415+36));
char *__tmp_string_1414="feel ugly Get to know your parents You never";
 __boundcheck_metadata_store((void *)(__tmp_string_1414),(void *)(__tmp_string_1414+44));
char *__tmp_string_1413="beauty magazines They will only make you";
 __boundcheck_metadata_store((void *)(__tmp_string_1413),(void *)(__tmp_string_1413+40));
char *__tmp_string_1412="you dont follow them Do not read";
 __boundcheck_metadata_store((void *)(__tmp_string_1412),(void *)(__tmp_string_1412+32));
char *__tmp_string_1411="room Read the directions even if";
 __boundcheck_metadata_store((void *)(__tmp_string_1411),(void *)(__tmp_string_1411+32));
char *__tmp_string_1410="nowhere to do it but your living";
 __boundcheck_metadata_store((void *)(__tmp_string_1410),(void *)(__tmp_string_1410+32));
char *__tmp_string_1409="own Dance even if you have";
 __boundcheck_metadata_store((void *)(__tmp_string_1409),(void *)(__tmp_string_1409+26));
char *__tmp_string_1408="greatest instrument youll ever";
 __boundcheck_metadata_store((void *)(__tmp_string_1408),(void *)(__tmp_string_1408+30));
char *__tmp_string_1407="the";
 __boundcheck_metadata_store((void *)(__tmp_string_1407),(void *)(__tmp_string_1407+3));
char *__tmp_string_1406="it Its";
 __boundcheck_metadata_store((void *)(__tmp_string_1406),(void *)(__tmp_string_1406+6));
char *__tmp_string_1405="other people think of";
 __boundcheck_metadata_store((void *)(__tmp_string_1405),(void *)(__tmp_string_1405+21));
char *__tmp_string_1404="be afraid of it or of what";
 __boundcheck_metadata_store((void *)(__tmp_string_1404),(void *)(__tmp_string_1404+26));
char *__tmp_string_1403="it every way you can Dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1403),(void *)(__tmp_string_1403+25));
char *__tmp_string_1402="Enjoy your body Use";
 __boundcheck_metadata_store((void *)(__tmp_string_1402),(void *)(__tmp_string_1402+19));
char *__tmp_string_1401="are everybody elses";
 __boundcheck_metadata_store((void *)(__tmp_string_1401),(void *)(__tmp_string_1401+19));
char *__tmp_string_1400="either Your choices are half chance So";
 __boundcheck_metadata_store((void *)(__tmp_string_1400),(void *)(__tmp_string_1400+38));
char *__tmp_string_1399="much or berate yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1399),(void *)(__tmp_string_1399+23));
char *__tmp_string_1398="you do dont congratulate yourself too";
 __boundcheck_metadata_store((void *)(__tmp_string_1398),(void *)(__tmp_string_1398+37));
char *__tmp_string_1397="your 75th wedding anniversary Whatever";
 __boundcheck_metadata_store((void *)(__tmp_string_1397),(void *)(__tmp_string_1397+38));
char *__tmp_string_1396="the funky chicken on";
 __boundcheck_metadata_store((void *)(__tmp_string_1396),(void *)(__tmp_string_1396+20));
char *__tmp_string_1395="you wont Maybe youll divorce at 40 maybe youll dance";
 __boundcheck_metadata_store((void *)(__tmp_string_1395),(void *)(__tmp_string_1395+52));
char *__tmp_string_1394="maybe you wont Maybe youll have children maybe";
 __boundcheck_metadata_store((void *)(__tmp_string_1394),(void *)(__tmp_string_1394+46));
char *__tmp_string_1393="Maybe youll marry";
 __boundcheck_metadata_store((void *)(__tmp_string_1393),(void *)(__tmp_string_1393+17));
char *__tmp_string_1392="them when theyre gone";
 __boundcheck_metadata_store((void *)(__tmp_string_1392),(void *)(__tmp_string_1392+21));
char *__tmp_string_1391="Be kind to your knees Youll miss";
 __boundcheck_metadata_store((void *)(__tmp_string_1391),(void *)(__tmp_string_1391+32));
char *__tmp_string_1390="Get plenty of calcium";
 __boundcheck_metadata_store((void *)(__tmp_string_1390),(void *)(__tmp_string_1390+21));
char *__tmp_string_1389="40yearolds I know still dont";
 __boundcheck_metadata_store((void *)(__tmp_string_1389),(void *)(__tmp_string_1389+28));
char *__tmp_string_1388="the most interesting";
 __boundcheck_metadata_store((void *)(__tmp_string_1388),(void *)(__tmp_string_1388+20));
char *__tmp_string_1387="to do with their lives Some of";
 __boundcheck_metadata_store((void *)(__tmp_string_1387),(void *)(__tmp_string_1387+30));
char *__tmp_string_1386="what they wanted";
 __boundcheck_metadata_store((void *)(__tmp_string_1386),(void *)(__tmp_string_1386+16));
char *__tmp_string_1385="know didnt know at 22";
 __boundcheck_metadata_store((void *)(__tmp_string_1385),(void *)(__tmp_string_1385+21));
char *__tmp_string_1384="life The most interesting people I";
 __boundcheck_metadata_store((void *)(__tmp_string_1384),(void *)(__tmp_string_1384+34));
char *__tmp_string_1383="what you want to do with your";
 __boundcheck_metadata_store((void *)(__tmp_string_1383),(void *)(__tmp_string_1383+29));
char *__tmp_string_1382="Stretch Dont feel guilty if you dont know";
 __boundcheck_metadata_store((void *)(__tmp_string_1382),(void *)(__tmp_string_1382+41));
char *__tmp_string_1381="your old bank statements";
 __boundcheck_metadata_store((void *)(__tmp_string_1381),(void *)(__tmp_string_1381+24));
char *__tmp_string_1380="love letters Throw away";
 __boundcheck_metadata_store((void *)(__tmp_string_1380),(void *)(__tmp_string_1380+23));
char *__tmp_string_1379="Keep your old";
 __boundcheck_metadata_store((void *)(__tmp_string_1379),(void *)(__tmp_string_1379+13));
char *__tmp_string_1378="succeed in doing this tell me how";
 __boundcheck_metadata_store((void *)(__tmp_string_1378),(void *)(__tmp_string_1378+33));
char *__tmp_string_1377="Forget the insults If you";
 __boundcheck_metadata_store((void *)(__tmp_string_1377),(void *)(__tmp_string_1377+25));
char *__tmp_string_1376="yourself Remember compliments you receive";
 __boundcheck_metadata_store((void *)(__tmp_string_1376),(void *)(__tmp_string_1376+41));
char *__tmp_string_1375="the end its only with";
 __boundcheck_metadata_store((void *)(__tmp_string_1375),(void *)(__tmp_string_1375+21));
char *__tmp_string_1374="The race is long and in";
 __boundcheck_metadata_store((void *)(__tmp_string_1374),(void *)(__tmp_string_1374+23));
char *__tmp_string_1373="on jealousy Sometimes youre ahead sometimes youre behind";
 __boundcheck_metadata_store((void *)(__tmp_string_1373),(void *)(__tmp_string_1373+56));
char *__tmp_string_1372="with yours Floss Dont waste your time";
 __boundcheck_metadata_store((void *)(__tmp_string_1372),(void *)(__tmp_string_1372+37));
char *__tmp_string_1371="with people who are reckless";
 __boundcheck_metadata_store((void *)(__tmp_string_1371),(void *)(__tmp_string_1371+28));
char *__tmp_string_1370="peoples hearts Dont put up";
 __boundcheck_metadata_store((void *)(__tmp_string_1370),(void *)(__tmp_string_1370+26));
char *__tmp_string_1369="scares you Sing Dont be reckless with other";
 __boundcheck_metadata_store((void *)(__tmp_string_1369),(void *)(__tmp_string_1369+43));
char *__tmp_string_1368="Do one thing every day that";
 __boundcheck_metadata_store((void *)(__tmp_string_1368),(void *)(__tmp_string_1368+27));
char *__tmp_string_1367="idle Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_1367),(void *)(__tmp_string_1367+12));
char *__tmp_string_1366="4 pm on some";
 __boundcheck_metadata_store((void *)(__tmp_string_1366),(void *)(__tmp_string_1366+12));
char *__tmp_string_1365="kind that blindside you at";
 __boundcheck_metadata_store((void *)(__tmp_string_1365),(void *)(__tmp_string_1365+26));
char *__tmp_string_1364="worried mind the";
 __boundcheck_metadata_store((void *)(__tmp_string_1364),(void *)(__tmp_string_1364+16));
char *__tmp_string_1363="be things that never crossed your";
 __boundcheck_metadata_store((void *)(__tmp_string_1363),(void *)(__tmp_string_1363+33));
char *__tmp_string_1362="your life are apt to";
 __boundcheck_metadata_store((void *)(__tmp_string_1362),(void *)(__tmp_string_1362+20));
char *__tmp_string_1361="by chewing bubble gum The real troubles in";
 __boundcheck_metadata_store((void *)(__tmp_string_1361),(void *)(__tmp_string_1361+42));
char *__tmp_string_1360="as trying to solve an algebra equation";
 __boundcheck_metadata_store((void *)(__tmp_string_1360),(void *)(__tmp_string_1360+38));
char *__tmp_string_1359="worry but know that worrying is as effective";
 __boundcheck_metadata_store((void *)(__tmp_string_1359),(void *)(__tmp_string_1359+44));
char *__tmp_string_1358="the future Or";
 __boundcheck_metadata_store((void *)(__tmp_string_1358),(void *)(__tmp_string_1358+13));
char *__tmp_string_1357="as you imagine Dont worry about";
 __boundcheck_metadata_store((void *)(__tmp_string_1357),(void *)(__tmp_string_1357+31));
char *__tmp_string_1356="are not as fat";
 __boundcheck_metadata_store((void *)(__tmp_string_1356),(void *)(__tmp_string_1356+14));
char *__tmp_string_1355="and how fabulous you really looked You";
 __boundcheck_metadata_store((void *)(__tmp_string_1355),(void *)(__tmp_string_1355+38));
char *__tmp_string_1354="possibility lay before you";
 __boundcheck_metadata_store((void *)(__tmp_string_1354),(void *)(__tmp_string_1354+26));
char *__tmp_string_1353="way you cant grasp now how much";
 __boundcheck_metadata_store((void *)(__tmp_string_1353),(void *)(__tmp_string_1353+31));
char *__tmp_string_1352="and recall in a";
 __boundcheck_metadata_store((void *)(__tmp_string_1352),(void *)(__tmp_string_1352+15));
char *__tmp_string_1351="back at photos of yourself";
 __boundcheck_metadata_store((void *)(__tmp_string_1351),(void *)(__tmp_string_1351+26));
char *__tmp_string_1350="youll look";
 __boundcheck_metadata_store((void *)(__tmp_string_1350),(void *)(__tmp_string_1350+10));
char *__tmp_string_1349="20 years";
 __boundcheck_metadata_store((void *)(__tmp_string_1349),(void *)(__tmp_string_1349+8));
char *__tmp_string_1348="faded But trust me in";
 __boundcheck_metadata_store((void *)(__tmp_string_1348),(void *)(__tmp_string_1348+21));
char *__tmp_string_1347="and beauty of your youth until theyve";
 __boundcheck_metadata_store((void *)(__tmp_string_1347),(void *)(__tmp_string_1347+37));
char *__tmp_string_1346="You will not understand the power";
 __boundcheck_metadata_store((void *)(__tmp_string_1346),(void *)(__tmp_string_1346+33));
char *__tmp_string_1345="of your youth Oh never mind";
 __boundcheck_metadata_store((void *)(__tmp_string_1345),(void *)(__tmp_string_1345+27));
char *__tmp_string_1344="now Enjoy the power and beauty";
 __boundcheck_metadata_store((void *)(__tmp_string_1344),(void *)(__tmp_string_1344+30));
char *__tmp_string_1343="I will dispense this advice";
 __boundcheck_metadata_store((void *)(__tmp_string_1343),(void *)(__tmp_string_1343+27));
char *__tmp_string_1342="more reliable than my own meandering experience";
 __boundcheck_metadata_store((void *)(__tmp_string_1342),(void *)(__tmp_string_1342+47));
char *__tmp_string_1341="my advice has no basis";
 __boundcheck_metadata_store((void *)(__tmp_string_1341),(void *)(__tmp_string_1341+22));
char *__tmp_string_1340="scientists whereas the rest of";
 __boundcheck_metadata_store((void *)(__tmp_string_1340),(void *)(__tmp_string_1340+30));
char *__tmp_string_1339="sunscreen have been proved by";
 __boundcheck_metadata_store((void *)(__tmp_string_1339),(void *)(__tmp_string_1339+29));
char *__tmp_string_1338="it The longterm benefits of";
 __boundcheck_metadata_store((void *)(__tmp_string_1338),(void *)(__tmp_string_1338+27));
char *__tmp_string_1337="the future sunscreen would be";
 __boundcheck_metadata_store((void *)(__tmp_string_1337),(void *)(__tmp_string_1337+29));
char *__tmp_string_1336="you only one tip for";
 __boundcheck_metadata_store((void *)(__tmp_string_1336),(void *)(__tmp_string_1336+20));
char *__tmp_string_1335="sunscreen If I could offer";
 __boundcheck_metadata_store((void *)(__tmp_string_1335),(void *)(__tmp_string_1335+26));
char *__tmp_string_1334="the class of 97 Wear";
 __boundcheck_metadata_store((void *)(__tmp_string_1334),(void *)(__tmp_string_1334+20));
char *__tmp_string_1333="MIT Ladies and gentlemen of";
 __boundcheck_metadata_store((void *)(__tmp_string_1333),(void *)(__tmp_string_1333+27));
char *__tmp_string_1332="Kurt Vonneguts Commencement Address at";
 __boundcheck_metadata_store((void *)(__tmp_string_1332),(void *)(__tmp_string_1332+38));
char *search_strings[] = { __tmp_string_1332,
__tmp_string_1333,
__tmp_string_1334,
__tmp_string_1335,
__tmp_string_1336,
__tmp_string_1337,
__tmp_string_1338,
__tmp_string_1339,
__tmp_string_1340,
__tmp_string_1341,
__tmp_string_1342,
__tmp_string_1343,
__tmp_string_1344,
__tmp_string_1345,
__tmp_string_1346,
__tmp_string_1347,
__tmp_string_1348,
__tmp_string_1349,
__tmp_string_1350,
__tmp_string_1351,
__tmp_string_1352,
__tmp_string_1353,
__tmp_string_1354,
__tmp_string_1355,
__tmp_string_1356,
__tmp_string_1357,
__tmp_string_1358,
__tmp_string_1359,
__tmp_string_1360,
__tmp_string_1361,
__tmp_string_1362,
__tmp_string_1363,
__tmp_string_1364,
__tmp_string_1365,
__tmp_string_1366,
__tmp_string_1367,
__tmp_string_1368,
__tmp_string_1369,
__tmp_string_1370,
__tmp_string_1371,
__tmp_string_1372,
__tmp_string_1373,
__tmp_string_1374,
__tmp_string_1375,
__tmp_string_1376,
__tmp_string_1377,
__tmp_string_1378,
__tmp_string_1379,
__tmp_string_1380,
__tmp_string_1381,
__tmp_string_1382,
__tmp_string_1383,
__tmp_string_1384,
__tmp_string_1385,
__tmp_string_1386,
__tmp_string_1387,
__tmp_string_1388,
__tmp_string_1389,
__tmp_string_1390,
__tmp_string_1391,
__tmp_string_1392,
__tmp_string_1393,
__tmp_string_1394,
__tmp_string_1395,
__tmp_string_1396,
__tmp_string_1397,
__tmp_string_1398,
__tmp_string_1399,
__tmp_string_1400,
__tmp_string_1401,
__tmp_string_1402,
__tmp_string_1403,
__tmp_string_1404,
__tmp_string_1405,
__tmp_string_1406,
__tmp_string_1407,
__tmp_string_1408,
__tmp_string_1409,
__tmp_string_1410,
__tmp_string_1411,
__tmp_string_1412,
__tmp_string_1413,
__tmp_string_1414,
__tmp_string_1415,
__tmp_string_1416,
__tmp_string_1417,
__tmp_string_1418,
__tmp_string_1419,
__tmp_string_1420,
__tmp_string_1421,
__tmp_string_1422,
__tmp_string_1423,
__tmp_string_1424,
__tmp_string_1425,
__tmp_string_1426,
__tmp_string_1427,
__tmp_string_1428,
__tmp_string_1429,
__tmp_string_1430,
__tmp_string_1431,
__tmp_string_1432,
__tmp_string_1433,
__tmp_string_1434,
__tmp_string_1435,
__tmp_string_1436,
__tmp_string_1437,
__tmp_string_1438,
__tmp_string_1439,
__tmp_string_1440,
__tmp_string_1441,
__tmp_string_1442,
__tmp_string_1443,
__tmp_string_1444,
__tmp_string_1445,
__tmp_string_1446,
__tmp_string_1447,
__tmp_string_1448,
__tmp_string_1449,
__tmp_string_1450,
__tmp_string_1451,
__tmp_string_1452,
__tmp_string_1453,
__tmp_string_1454,
__tmp_string_1455,
__tmp_string_1456,
__tmp_string_1457,
__tmp_string_1458,
__tmp_string_1459,
__tmp_string_1460,
__tmp_string_1461,
__tmp_string_1462,
__tmp_string_1463,
__tmp_string_1464,
__tmp_string_1465,
__tmp_string_1466,
__tmp_string_1467,
__tmp_string_1468,
__tmp_string_1469,
__tmp_string_1470,
__tmp_string_1471,
__tmp_string_1472,
__tmp_string_1473,
__tmp_string_1474,
__tmp_string_1475,
__tmp_string_1476,
__tmp_string_1477,
__tmp_string_1478,
__tmp_string_1479,
__tmp_string_1480,
__tmp_string_1481,
__tmp_string_1482,
__tmp_string_1483,
__tmp_string_1484,
__tmp_string_1485,
__tmp_string_1486,
__tmp_string_1487,
__tmp_string_1488,
__tmp_string_1489,
__tmp_string_1490,
__tmp_string_1491,
__tmp_string_1492,
__tmp_string_1493,
__tmp_string_1494,
__tmp_string_1495,
__tmp_string_1496,
__tmp_string_1497,
__tmp_string_1498,
__tmp_string_1499,
__tmp_string_1500,
__tmp_string_1501,
__tmp_string_1502,
__tmp_string_1503,
__tmp_string_1504,
__tmp_string_1505,
__tmp_string_1506,
__tmp_string_1507,
__tmp_string_1508,
__tmp_string_1509,
__tmp_string_1510,
__tmp_string_1511,
__tmp_string_1512,
__tmp_string_1513,
__tmp_string_1514,
__tmp_string_1515,
__tmp_string_1516,
__tmp_string_1517,
__tmp_string_1518,
__tmp_string_1519,
__tmp_string_1520,
__tmp_string_1521,
__tmp_string_1522,
__tmp_string_1523,
__tmp_string_1524,
__tmp_string_1525,
__tmp_string_1526,
__tmp_string_1527,
__tmp_string_1528,
__tmp_string_1529,
__tmp_string_1530,
__tmp_string_1531,
__tmp_string_1532,
__tmp_string_1533,
__tmp_string_1534,
__tmp_string_1535,
__tmp_string_1536,
__tmp_string_1537,
__tmp_string_1538,
__tmp_string_1539,
__tmp_string_1540,
__tmp_string_1541,
__tmp_string_1542,
__tmp_string_1543,
__tmp_string_1544,
__tmp_string_1545,
__tmp_string_1546,
__tmp_string_1547,
__tmp_string_1548,
__tmp_string_1549,
__tmp_string_1550,
__tmp_string_1551,
__tmp_string_1552,
__tmp_string_1553,
__tmp_string_1554,
__tmp_string_1555,
__tmp_string_1556,
__tmp_string_1557,
__tmp_string_1558,
__tmp_string_1559,
__tmp_string_1560,
__tmp_string_1561,
__tmp_string_1562,
__tmp_string_1563,
__tmp_string_1564,
__tmp_string_1565,
__tmp_string_1566,
__tmp_string_1567,
__tmp_string_1568,
__tmp_string_1569,
__tmp_string_1570,
__tmp_string_1571,
__tmp_string_1572,
__tmp_string_1573,
__tmp_string_1574,
__tmp_string_1575,
__tmp_string_1576,
__tmp_string_1577,
__tmp_string_1578,
__tmp_string_1579,
__tmp_string_1580,
__tmp_string_1581,
__tmp_string_1582,
__tmp_string_1583,
__tmp_string_1584,
__tmp_string_1585,
__tmp_string_1586,
__tmp_string_1587,
__tmp_string_1588,
__tmp_string_1589,
__tmp_string_1590,
__tmp_string_1591,
__tmp_string_1592,
__tmp_string_1593,
__tmp_string_1594,
__tmp_string_1595,
__tmp_string_1596,
__tmp_string_1597,
__tmp_string_1598,
__tmp_string_1599,
__tmp_string_1600,
__tmp_string_1601,
__tmp_string_1602,
__tmp_string_1603,
__tmp_string_1604,
__tmp_string_1605,
__tmp_string_1606,
__tmp_string_1607,
__tmp_string_1608,
__tmp_string_1609,
__tmp_string_1610,
__tmp_string_1611,
__tmp_string_1612,
__tmp_string_1613,
__tmp_string_1614,
__tmp_string_1615,
__tmp_string_1616,
__tmp_string_1617,
__tmp_string_1618,
__tmp_string_1619,
__tmp_string_1620,
__tmp_string_1621,
__tmp_string_1622,
__tmp_string_1623,
__tmp_string_1624,
__tmp_string_1625,
__tmp_string_1626,
__tmp_string_1627,
__tmp_string_1628,
__tmp_string_1629,
__tmp_string_1630,
__tmp_string_1631,
__tmp_string_1632,
__tmp_string_1633,
__tmp_string_1634,
__tmp_string_1635,
__tmp_string_1636,
__tmp_string_1637,
__tmp_string_1638,
__tmp_string_1639,
__tmp_string_1640,
__tmp_string_1641,
__tmp_string_1642,
__tmp_string_1643,
__tmp_string_1644,
__tmp_string_1645,
__tmp_string_1646,
__tmp_string_1647,
__tmp_string_1648,
__tmp_string_1649,
__tmp_string_1650,
__tmp_string_1651,
__tmp_string_1652,
__tmp_string_1653,
__tmp_string_1654,
__tmp_string_1655,
__tmp_string_1656,
__tmp_string_1657,
__tmp_string_1658,
__tmp_string_1659,
__tmp_string_1660,
__tmp_string_1661,
__tmp_string_1662,
__tmp_string_1663,
__tmp_string_1664,
__tmp_string_1665,
__tmp_string_1666,
__tmp_string_1667,
__tmp_string_1668,
__tmp_string_1669,
__tmp_string_1670,
__tmp_string_1671,
__tmp_string_1672,
__tmp_string_1673,
__tmp_string_1674,
__tmp_string_1675,
__tmp_string_1676,
__tmp_string_1677,
__tmp_string_1678,
__tmp_string_1679,
__tmp_string_1680,
__tmp_string_1681,
__tmp_string_1682,
__tmp_string_1683,
__tmp_string_1684,
__tmp_string_1685,
__tmp_string_1686,
__tmp_string_1687,
__tmp_string_1688,
__tmp_string_1689,
__tmp_string_1690,
__tmp_string_1691,
__tmp_string_1692,
__tmp_string_1693,
__tmp_string_1694,
__tmp_string_1695,
__tmp_string_1696,
__tmp_string_1697,
__tmp_string_1698,
__tmp_string_1699,
__tmp_string_1700,
__tmp_string_1701,
__tmp_string_1702,
__tmp_string_1703,
__tmp_string_1704,
__tmp_string_1705,
__tmp_string_1706,
__tmp_string_1707,
__tmp_string_1708,
__tmp_string_1709,
__tmp_string_1710,
__tmp_string_1711,
__tmp_string_1712,
__tmp_string_1713,
__tmp_string_1714,
__tmp_string_1715,
__tmp_string_1716,
__tmp_string_1717,
__tmp_string_1718,
__tmp_string_1719,
__tmp_string_1720,
__tmp_string_1721,
__tmp_string_1722,
__tmp_string_1723,
__tmp_string_1724,
__tmp_string_1725,
__tmp_string_1726,
__tmp_string_1727,
__tmp_string_1728,
__tmp_string_1729,
__tmp_string_1730,
__tmp_string_1731,
__tmp_string_1732,
__tmp_string_1733,
__tmp_string_1734,
__tmp_string_1735,
__tmp_string_1736,
__tmp_string_1737,
__tmp_string_1738,
__tmp_string_1739,
__tmp_string_1740,
__tmp_string_1741,
__tmp_string_1742,
__tmp_string_1743,
__tmp_string_1744,
__tmp_string_1745,
__tmp_string_1746,
__tmp_string_1747,
__tmp_string_1748,
__tmp_string_1749,
__tmp_string_1750,
__tmp_string_1751,
__tmp_string_1752,
__tmp_string_1753,
__tmp_string_1754,
__tmp_string_1755,
__tmp_string_1756,
__tmp_string_1757,
__tmp_string_1758,
__tmp_string_1759,
__tmp_string_1760,
__tmp_string_1761,
__tmp_string_1762,
__tmp_string_1763,
__tmp_string_1764,
__tmp_string_1765,
__tmp_string_1766,
__tmp_string_1767,
__tmp_string_1768,
__tmp_string_1769,
__tmp_string_1770,
__tmp_string_1771,
__tmp_string_1772,
__tmp_string_1773,
__tmp_string_1774,
__tmp_string_1775,
__tmp_string_1776,
__tmp_string_1777,
__tmp_string_1778,
__tmp_string_1779,
__tmp_string_1780,
__tmp_string_1781,
__tmp_string_1782,
__tmp_string_1783,
__tmp_string_1784,
__tmp_string_1785,
__tmp_string_1786,
__tmp_string_1787,
__tmp_string_1788,
__tmp_string_1789,
__tmp_string_1790,
__tmp_string_1791,
__tmp_string_1792,
__tmp_string_1793,
__tmp_string_1794,
__tmp_string_1795,
__tmp_string_1796,
__tmp_string_1797,
__tmp_string_1798,
__tmp_string_1799,
__tmp_string_1800,
__tmp_string_1801,
__tmp_string_1802,
__tmp_string_1803,
__tmp_string_1804,
__tmp_string_1805,
__tmp_string_1806,
__tmp_string_1807,
__tmp_string_1808,
__tmp_string_1809,
__tmp_string_1810,
__tmp_string_1811,
__tmp_string_1812,
__tmp_string_1813,
__tmp_string_1814,
__tmp_string_1815,
__tmp_string_1816,
__tmp_string_1817,
__tmp_string_1818,
__tmp_string_1819,
__tmp_string_1820,
__tmp_string_1821,
__tmp_string_1822,
__tmp_string_1823,
__tmp_string_1824,
__tmp_string_1825,
__tmp_string_1826,
__tmp_string_1827,
__tmp_string_1828,
__tmp_string_1829,
__tmp_string_1830,
__tmp_string_1831,
__tmp_string_1832,
__tmp_string_1833,
__tmp_string_1834,
__tmp_string_1835,
__tmp_string_1836,
__tmp_string_1837,
__tmp_string_1838,
__tmp_string_1839,
__tmp_string_1840,
__tmp_string_1841,
__tmp_string_1842,
__tmp_string_1843,
__tmp_string_1844,
__tmp_string_1845,
__tmp_string_1846,
__tmp_string_1847,
__tmp_string_1848,
__tmp_string_1849,
__tmp_string_1850,
__tmp_string_1851,
__tmp_string_1852,
__tmp_string_1853,
__tmp_string_1854,
__tmp_string_1855,
__tmp_string_1856,
__tmp_string_1857,
__tmp_string_1858,
__tmp_string_1859,
__tmp_string_1860,
__tmp_string_1861,
__tmp_string_1862,
__tmp_string_1863,
__tmp_string_1864,
__tmp_string_1865,
__tmp_string_1866,
__tmp_string_1867,
__tmp_string_1868,
__tmp_string_1869,
__tmp_string_1870,
__tmp_string_1871,
__tmp_string_1872,
__tmp_string_1873,
__tmp_string_1874,
__tmp_string_1875,
__tmp_string_1876,
__tmp_string_1877,
__tmp_string_1878,
__tmp_string_1879,
__tmp_string_1880,
__tmp_string_1881,
__tmp_string_1882,
__tmp_string_1883,
__tmp_string_1884,
__tmp_string_1885,
__tmp_string_1886,
__tmp_string_1887,
__tmp_string_1888,
__tmp_string_1889,
__tmp_string_1890,
__tmp_string_1891,
__tmp_string_1892,
__tmp_string_1893,
__tmp_string_1894,
__tmp_string_1895,
__tmp_string_1896,
__tmp_string_1897,
__tmp_string_1898,
__tmp_string_1899,
__tmp_string_1900,
__tmp_string_1901,
__tmp_string_1902,
__tmp_string_1903,
__tmp_string_1904,
__tmp_string_1905,
__tmp_string_1906,
__tmp_string_1907,
__tmp_string_1908,
__tmp_string_1909,
__tmp_string_1910,
__tmp_string_1911,
__tmp_string_1912,
__tmp_string_1913,
__tmp_string_1914,
__tmp_string_1915,
__tmp_string_1916,
__tmp_string_1917,
__tmp_string_1918,
__tmp_string_1919,
__tmp_string_1920,
__tmp_string_1921,
__tmp_string_1922,
__tmp_string_1923,
__tmp_string_1924,
__tmp_string_1925,
__tmp_string_1926,
__tmp_string_1927,
__tmp_string_1928,
__tmp_string_1929,
__tmp_string_1930,
__tmp_string_1931,
__tmp_string_1932,
__tmp_string_1933,
__tmp_string_1934,
__tmp_string_1935,
__tmp_string_1936,
__tmp_string_1937,
__tmp_string_1938,
__tmp_string_1939,
__tmp_string_1940,
__tmp_string_1941,
__tmp_string_1942,
__tmp_string_1943,
__tmp_string_1944,
__tmp_string_1945,
__tmp_string_1946,
__tmp_string_1947,
__tmp_string_1948,
__tmp_string_1949,
__tmp_string_1950,
__tmp_string_1951,
__tmp_string_1952,
__tmp_string_1953,
__tmp_string_1954,
__tmp_string_1955,
__tmp_string_1956,
__tmp_string_1957,
__tmp_string_1958,
__tmp_string_1959,
__tmp_string_1960,
__tmp_string_1961,
__tmp_string_1962,
__tmp_string_1963,
__tmp_string_1964,
__tmp_string_1965,
__tmp_string_1966,
__tmp_string_1967,
__tmp_string_1968,
__tmp_string_1969,
__tmp_string_1970,
__tmp_string_1971,
__tmp_string_1972,
__tmp_string_1973,
__tmp_string_1974,
__tmp_string_1975,
__tmp_string_1976,
__tmp_string_1977,
__tmp_string_1978,
__tmp_string_1979,
__tmp_string_1980,
__tmp_string_1981,
__tmp_string_1982,
__tmp_string_1983,
__tmp_string_1984,
__tmp_string_1985,
__tmp_string_1986,
__tmp_string_1987,
__tmp_string_1988,
__tmp_string_1989,
__tmp_string_1990,
__tmp_string_1991,
__tmp_string_1992,
__tmp_string_1993,
__tmp_string_1994,
__tmp_string_1995,
__tmp_string_1996,
__tmp_string_1997,
__tmp_string_1998,
__tmp_string_1999,
__tmp_string_2000,
__tmp_string_2001,
__tmp_string_2002,
__tmp_string_2003,
__tmp_string_2004,
__tmp_string_2005,
__tmp_string_2006,
__tmp_string_2007,
__tmp_string_2008,
__tmp_string_2009,
__tmp_string_2010,
__tmp_string_2011,
__tmp_string_2012,
__tmp_string_2013,
__tmp_string_2014,
__tmp_string_2015,
__tmp_string_2016,
__tmp_string_2017,
__tmp_string_2018,
__tmp_string_2019,
__tmp_string_2020,
__tmp_string_2021,
__tmp_string_2022,
__tmp_string_2023,
__tmp_string_2024,
__tmp_string_2025,
__tmp_string_2026,
__tmp_string_2027,
__tmp_string_2028,
__tmp_string_2029,
__tmp_string_2030,
__tmp_string_2031,
__tmp_string_2032,
__tmp_string_2033,
__tmp_string_2034,
__tmp_string_2035,
__tmp_string_2036,
__tmp_string_2037,
__tmp_string_2038,
__tmp_string_2039,
__tmp_string_2040,
__tmp_string_2041,
__tmp_string_2042,
__tmp_string_2043,
__tmp_string_2044,
__tmp_string_2045,
__tmp_string_2046,
__tmp_string_2047,
__tmp_string_2048,
__tmp_string_2049,
__tmp_string_2050,
__tmp_string_2051,
__tmp_string_2052,
__tmp_string_2053,
__tmp_string_2054,
__tmp_string_2055,
__tmp_string_2056,
__tmp_string_2057,
__tmp_string_2058,
__tmp_string_2059,
__tmp_string_2060,
__tmp_string_2061,
__tmp_string_2062,
__tmp_string_2063,
__tmp_string_2064,
__tmp_string_2065,
__tmp_string_2066,
__tmp_string_2067,
__tmp_string_2068,
__tmp_string_2069,
__tmp_string_2070,
__tmp_string_2071,
__tmp_string_2072,
__tmp_string_2073,
__tmp_string_2074,
__tmp_string_2075,
__tmp_string_2076,
__tmp_string_2077,
__tmp_string_2078,
__tmp_string_2079,
__tmp_string_2080,
__tmp_string_2081,
__tmp_string_2082,
__tmp_string_2083,
__tmp_string_2084,
__tmp_string_2085,
__tmp_string_2086,
__tmp_string_2087,
__tmp_string_2088,
__tmp_string_2089,
__tmp_string_2090,
__tmp_string_2091,
__tmp_string_2092,
__tmp_string_2093,
__tmp_string_2094,
__tmp_string_2095,
__tmp_string_2096,
__tmp_string_2097,
__tmp_string_2098,
__tmp_string_2099,
__tmp_string_2100,
__tmp_string_2101,
__tmp_string_2102,
__tmp_string_2103,
__tmp_string_2104,
__tmp_string_2105,
__tmp_string_2106,
__tmp_string_2107,
__tmp_string_2108,
__tmp_string_2109,
__tmp_string_2110,
__tmp_string_2111,
__tmp_string_2112,
__tmp_string_2113,
__tmp_string_2114,
__tmp_string_2115,
__tmp_string_2116,
__tmp_string_2117,
__tmp_string_2118,
__tmp_string_2119,
__tmp_string_2120,
__tmp_string_2121,
__tmp_string_2122,
__tmp_string_2123,
__tmp_string_2124,
__tmp_string_2125,
__tmp_string_2126,
__tmp_string_2127,
__tmp_string_2128,
__tmp_string_2129,
__tmp_string_2130,
__tmp_string_2131,
__tmp_string_2132,
__tmp_string_2133,
__tmp_string_2134,
__tmp_string_2135,
__tmp_string_2136,
__tmp_string_2137,
__tmp_string_2138,
__tmp_string_2139,
__tmp_string_2140,
__tmp_string_2141,
__tmp_string_2142,
__tmp_string_2143,
__tmp_string_2144,
__tmp_string_2145,
__tmp_string_2146,
__tmp_string_2147,
__tmp_string_2148,
__tmp_string_2149,
__tmp_string_2150,
__tmp_string_2151,
__tmp_string_2152,
__tmp_string_2153,
__tmp_string_2154,
__tmp_string_2155,
__tmp_string_2156,
__tmp_string_2157,
__tmp_string_2158,
__tmp_string_2159,
__tmp_string_2160,
__tmp_string_2161,
__tmp_string_2162,
__tmp_string_2163,
__tmp_string_2164,
__tmp_string_2165,
__tmp_string_2166,
__tmp_string_2167,
__tmp_string_2168,
__tmp_string_2169,
__tmp_string_2170,
__tmp_string_2171,
__tmp_string_2172,
__tmp_string_2173,
__tmp_string_2174,
__tmp_string_2175,
__tmp_string_2176,
__tmp_string_2177,
__tmp_string_2178,
__tmp_string_2179,
__tmp_string_2180,
__tmp_string_2181,
__tmp_string_2182,
__tmp_string_2183,
__tmp_string_2184,
__tmp_string_2185,
__tmp_string_2186,
__tmp_string_2187,
__tmp_string_2188,
__tmp_string_2189,
__tmp_string_2190,
__tmp_string_2191,
__tmp_string_2192,
__tmp_string_2193,
__tmp_string_2194,
__tmp_string_2195,
__tmp_string_2196,
__tmp_string_2197,
__tmp_string_2198,
__tmp_string_2199,
__tmp_string_2200,
__tmp_string_2201,
__tmp_string_2202,
__tmp_string_2203,
__tmp_string_2204,
__tmp_string_2205,
__tmp_string_2206,
__tmp_string_2207,
__tmp_string_2208,
__tmp_string_2209,
__tmp_string_2210,
__tmp_string_2211,
__tmp_string_2212,
__tmp_string_2213,
__tmp_string_2214,
__tmp_string_2215,
__tmp_string_2216,
__tmp_string_2217,
__tmp_string_2218,
__tmp_string_2219,
__tmp_string_2220,
__tmp_string_2221,
__tmp_string_2222,
__tmp_string_2223,
__tmp_string_2224,
__tmp_string_2225,
__tmp_string_2226,
__tmp_string_2227,
__tmp_string_2228,
__tmp_string_2229,
__tmp_string_2230,
__tmp_string_2231,
__tmp_string_2232,
__tmp_string_2233,
__tmp_string_2234,
__tmp_string_2235,
__tmp_string_2236,
__tmp_string_2237,
__tmp_string_2238,
__tmp_string_2239,
__tmp_string_2240,
__tmp_string_2241,
__tmp_string_2242,
__tmp_string_2243,
__tmp_string_2244,
__tmp_string_2245,
__tmp_string_2246,
__tmp_string_2247,
__tmp_string_2248,
__tmp_string_2249,
__tmp_string_2250,
__tmp_string_2251,
__tmp_string_2252,
__tmp_string_2253,
__tmp_string_2254,
__tmp_string_2255,
__tmp_string_2256,
__tmp_string_2257,
__tmp_string_2258,
__tmp_string_2259,
__tmp_string_2260,
__tmp_string_2261,
__tmp_string_2262,
__tmp_string_2263,
__tmp_string_2264,
__tmp_string_2265,
__tmp_string_2266,
__tmp_string_2267,
__tmp_string_2268,
__tmp_string_2269,
__tmp_string_2270,
__tmp_string_2271,
__tmp_string_2272,
__tmp_string_2273,
__tmp_string_2274,
__tmp_string_2275,
__tmp_string_2276,
__tmp_string_2277,
__tmp_string_2278,
__tmp_string_2279,
__tmp_string_2280,
__tmp_string_2281,
__tmp_string_2282,
__tmp_string_2283,
__tmp_string_2284,
__tmp_string_2285,
__tmp_string_2286,
__tmp_string_2287,
__tmp_string_2288,
__tmp_string_2289,
__tmp_string_2290,
__tmp_string_2291,
__tmp_string_2292,
__tmp_string_2293,
__tmp_string_2294,
__tmp_string_2295,
__tmp_string_2296,
__tmp_string_2297,
__tmp_string_2298,
__tmp_string_2299,
__tmp_string_2300,
__tmp_string_2301,
__tmp_string_2302,
__tmp_string_2303,
__tmp_string_2304,
__tmp_string_2305,
__tmp_string_2306,
__tmp_string_2307,
__tmp_string_2308,
__tmp_string_2309,
__tmp_string_2310,
__tmp_string_2311,
__tmp_string_2312,
__tmp_string_2313,
__tmp_string_2314,
__tmp_string_2315,
__tmp_string_2316,
__tmp_string_2317,
__tmp_string_2318,
__tmp_string_2319,
__tmp_string_2320,
__tmp_string_2321,
__tmp_string_2322,
__tmp_string_2323,
__tmp_string_2324,
__tmp_string_2325,
__tmp_string_2326,
__tmp_string_2327,
__tmp_string_2328,
__tmp_string_2329,
__tmp_string_2330,
__tmp_string_2331,
__tmp_string_2332,
__tmp_string_2333,
__tmp_string_2334,
__tmp_string_2335,
__tmp_string_2336,
__tmp_string_2337,
__tmp_string_2338,
__tmp_string_2339,
__tmp_string_2340,
__tmp_string_2341,
__tmp_string_2342,
__tmp_string_2343,
__tmp_string_2344,
__tmp_string_2345,
__tmp_string_2346,
__tmp_string_2347,
__tmp_string_2348,
__tmp_string_2349,
__tmp_string_2350,
__tmp_string_2351,
__tmp_string_2352,
__tmp_string_2353,
__tmp_string_2354,
__tmp_string_2355,
__tmp_string_2356,
__tmp_string_2357,
__tmp_string_2358,
__tmp_string_2359,
__tmp_string_2360,
__tmp_string_2361,
__tmp_string_2362,
__tmp_string_2363,
__tmp_string_2364,
__tmp_string_2365,
__tmp_string_2366,
__tmp_string_2367,
__tmp_string_2368,
__tmp_string_2369,
__tmp_string_2370,
__tmp_string_2371,
__tmp_string_2372,
__tmp_string_2373,
__tmp_string_2374,
__tmp_string_2375,
__tmp_string_2376,
__tmp_string_2377,
__tmp_string_2378,
__tmp_string_2379,
__tmp_string_2380,
__tmp_string_2381,
__tmp_string_2382,
__tmp_string_2383,
__tmp_string_2384,
__tmp_string_2385,
__tmp_string_2386,
__tmp_string_2387,
__tmp_string_2388,
__tmp_string_2389,
__tmp_string_2390,
__tmp_string_2391,
__tmp_string_2392,
__tmp_string_2393,
__tmp_string_2394,
__tmp_string_2395,
__tmp_string_2396,
__tmp_string_2397,
__tmp_string_2398,
__tmp_string_2399,
__tmp_string_2400,
__tmp_string_2401,
__tmp_string_2402,
__tmp_string_2403,
__tmp_string_2404,
__tmp_string_2405,
__tmp_string_2406,
__tmp_string_2407,
__tmp_string_2408,
__tmp_string_2409,
__tmp_string_2410,
__tmp_string_2411,
__tmp_string_2412,
__tmp_string_2413,
__tmp_string_2414,
__tmp_string_2415,
__tmp_string_2416,
__tmp_string_2417,
__tmp_string_2418,
__tmp_string_2419,
__tmp_string_2420,
__tmp_string_2421,
__tmp_string_2422,
__tmp_string_2423,
__tmp_string_2424,
__tmp_string_2425,
__tmp_string_2426,
__tmp_string_2427,
__tmp_string_2428,
__tmp_string_2429,
__tmp_string_2430,
__tmp_string_2431,
__tmp_string_2432,
__tmp_string_2433,
__tmp_string_2434,
__tmp_string_2435,
__tmp_string_2436,
__tmp_string_2437,
__tmp_string_2438,
__tmp_string_2439,
__tmp_string_2440,
__tmp_string_2441,
__tmp_string_2442,
__tmp_string_2443,
__tmp_string_2444,
__tmp_string_2445,
__tmp_string_2446,
__tmp_string_2447,
__tmp_string_2448,
__tmp_string_2449,
__tmp_string_2450,
__tmp_string_2451,
__tmp_string_2452,
__tmp_string_2453,
__tmp_string_2454,
__tmp_string_2455,
__tmp_string_2456,
__tmp_string_2457,
__tmp_string_2458,
__tmp_string_2459,
__tmp_string_2460,
__tmp_string_2461,
__tmp_string_2462,
__tmp_string_2463,
__tmp_string_2464,
__tmp_string_2465,
__tmp_string_2466,
__tmp_string_2467,
__tmp_string_2468,
__tmp_string_2469,
__tmp_string_2470,
__tmp_string_2471,
__tmp_string_2472,
__tmp_string_2473,
__tmp_string_2474,
__tmp_string_2475,
__tmp_string_2476,
__tmp_string_2477,
__tmp_string_2478,
__tmp_string_2479,
__tmp_string_2480,
__tmp_string_2481,
__tmp_string_2482,
__tmp_string_2483,
__tmp_string_2484,
__tmp_string_2485,
__tmp_string_2486,
__tmp_string_2487,
__tmp_string_2488,
__tmp_string_2489,
__tmp_string_2490,
__tmp_string_2491,
__tmp_string_2492,
__tmp_string_2493,
__tmp_string_2494,
__tmp_string_2495,
__tmp_string_2496,
__tmp_string_2497,
__tmp_string_2498,
__tmp_string_2499,
__tmp_string_2500,
__tmp_string_2501,
__tmp_string_2502,
__tmp_string_2503,
__tmp_string_2504,
__tmp_string_2505,
__tmp_string_2506,
__tmp_string_2507,
__tmp_string_2508,
__tmp_string_2509,
__tmp_string_2510,
__tmp_string_2511,
__tmp_string_2512,
__tmp_string_2513,
__tmp_string_2514,
__tmp_string_2515,
__tmp_string_2516,
__tmp_string_2517,
__tmp_string_2518,
__tmp_string_2519,
__tmp_string_2520,
__tmp_string_2521,
__tmp_string_2522,
__tmp_string_2523,
__tmp_string_2524,
__tmp_string_2525,
__tmp_string_2526,
__tmp_string_2527,
__tmp_string_2528,
__tmp_string_2529,
__tmp_string_2530,
__tmp_string_2531,
__tmp_string_2532,
__tmp_string_2533,
__tmp_string_2534,
__tmp_string_2535,
__tmp_string_2536,
__tmp_string_2537,
__tmp_string_2538,
__tmp_string_2539,
__tmp_string_2540,
__tmp_string_2541,
__tmp_string_2542,
__tmp_string_2543,
__tmp_string_2544,
__tmp_string_2545,
__tmp_string_2546,
__tmp_string_2547,
__tmp_string_2548,
__tmp_string_2549,
__tmp_string_2550,
__tmp_string_2551,
__tmp_string_2552,
__tmp_string_2553,
__tmp_string_2554,
__tmp_string_2555,
__tmp_string_2556,
__tmp_string_2557,
__tmp_string_2558,
__tmp_string_2559,
__tmp_string_2560,
__tmp_string_2561,
__tmp_string_2562,
__tmp_string_2563,
__tmp_string_2564,
__tmp_string_2565,
__tmp_string_2566,
__tmp_string_2567,
__tmp_string_2568,
__tmp_string_2569,
__tmp_string_2570,
__tmp_string_2571,
__tmp_string_2572,
__tmp_string_2573,
__tmp_string_2574,
__tmp_string_2575,
__tmp_string_2576,
__tmp_string_2577,
__tmp_string_2578,
__tmp_string_2579,
__tmp_string_2580,
__tmp_string_2581,
__tmp_string_2582,
__tmp_string_2583,
__tmp_string_2584,
__tmp_string_2585,
__tmp_string_2586,
__tmp_string_2587,
__tmp_string_2588,
__tmp_string_2589,
__tmp_string_2590,
__tmp_string_2591,
__tmp_string_2592,
__tmp_string_2593,
__tmp_string_2594,
__tmp_string_2595,
__tmp_string_2596,
__tmp_string_2597,
__tmp_string_2598,
__tmp_string_2599,
__tmp_string_2600,
__tmp_string_2601,
__tmp_string_2602,
__tmp_string_2603,
__tmp_string_2604,
__tmp_string_2605,
__tmp_string_2606,
__tmp_string_2607,
__tmp_string_2608,
__tmp_string_2609,
__tmp_string_2610,
__tmp_string_2611,
__tmp_string_2612,
__tmp_string_2613,
__tmp_string_2614,
__tmp_string_2615,
__tmp_string_2616,
__tmp_string_2617,
__tmp_string_2618,
__tmp_string_2619,
__tmp_string_2620,
__tmp_string_2621,
__tmp_string_2622,
__tmp_string_2623,
__tmp_string_2624,
__tmp_string_2625,
__tmp_string_2626,
__tmp_string_2627,
__tmp_string_2628,
__tmp_string_2629,
__tmp_string_2630,
__tmp_string_2631,
__tmp_string_2632,
__tmp_string_2633,
__tmp_string_2634,
__tmp_string_2635,
__tmp_string_2636,
__tmp_string_2637,
__tmp_string_2638,
__tmp_string_2639,
__tmp_string_2640,
__tmp_string_2641,
__tmp_string_2642,
__tmp_string_2643,
__tmp_string_2644,
__tmp_string_2645,
__tmp_string_2646,
__tmp_string_2647,
__tmp_string_2648,
__tmp_string_2649,
__tmp_string_2650,
__tmp_string_2651,
__tmp_string_2652,
__tmp_string_2653,
__tmp_string_2654,
__tmp_string_2655,
__tmp_string_2656,
__tmp_string_2657,
__tmp_string_2658,
__tmp_string_2659,
__tmp_string_2660,
__tmp_string_2661,
__tmp_string_2662,
__tmp_string_2663
};
__boundcheck_metadata_store((void *)(&search_strings),(void *)((size_t)(&search_strings)+sizeof(search_strings)*8-1));

      int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


      for (i = 0; find_strings[_RV_insert_check(0,1333,2743,32,"main",i)]; i++)
      {
            init_search(find_strings[_RV_insert_check(0,1333,2745,38,"main",i)]);
            here = strsearch(search_strings[_RV_insert_check(0,1332,2746,45,"main",i)]);
            printf("\"%s\" is%s in \"%s\"", find_strings[_RV_insert_check(0,1333,2747,58,"main",i)],
                  here ? "" : " not", search_strings[_RV_insert_check(0,1332,2748,54,"main",i)]);
            if (here)
                  printf(" [\"%s\"]", here);
            putchar('\n');
      }

      return 0;
}

