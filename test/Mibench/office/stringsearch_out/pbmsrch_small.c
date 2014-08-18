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

      char *__tmp_string_56="principalities";
 __boundcheck_metadata_store((void *)(__tmp_string_56),(void *)(__tmp_string_56+14));
char *__tmp_string_55="implement";
 __boundcheck_metadata_store((void *)(__tmp_string_55),(void *)(__tmp_string_55+9));
char *__tmp_string_54="officers";
 __boundcheck_metadata_store((void *)(__tmp_string_54),(void *)(__tmp_string_54+8));
char *__tmp_string_53="regard";
 __boundcheck_metadata_store((void *)(__tmp_string_53),(void *)(__tmp_string_53+6));
char *__tmp_string_52="guide";
 __boundcheck_metadata_store((void *)(__tmp_string_52),(void *)(__tmp_string_52+5));
char *__tmp_string_51="such";
 __boundcheck_metadata_store((void *)(__tmp_string_51),(void *)(__tmp_string_51+4));
char *__tmp_string_50="member";
 __boundcheck_metadata_store((void *)(__tmp_string_50),(void *)(__tmp_string_50+6));
char *__tmp_string_49="the";
 __boundcheck_metadata_store((void *)(__tmp_string_49),(void *)(__tmp_string_49+3));
char *__tmp_string_48="circumstance";
 __boundcheck_metadata_store((void *)(__tmp_string_48),(void *)(__tmp_string_48+12));
char *__tmp_string_47="spaces";
 __boundcheck_metadata_store((void *)(__tmp_string_47),(void *)(__tmp_string_47+6));
char *__tmp_string_46="thats";
 __boundcheck_metadata_store((void *)(__tmp_string_46),(void *)(__tmp_string_46+5));
char *__tmp_string_45="describedly";
 __boundcheck_metadata_store((void *)(__tmp_string_45),(void *)(__tmp_string_45+11));
char *__tmp_string_44="involve";
 __boundcheck_metadata_store((void *)(__tmp_string_44),(void *)(__tmp_string_44+7));
char *__tmp_string_43="proper";
 __boundcheck_metadata_store((void *)(__tmp_string_43),(void *)(__tmp_string_43+6));
char *__tmp_string_42="have";
 __boundcheck_metadata_store((void *)(__tmp_string_42),(void *)(__tmp_string_42+4));
char *__tmp_string_41="about";
 __boundcheck_metadata_store((void *)(__tmp_string_41),(void *)(__tmp_string_41+5));
char *__tmp_string_40="struct";
 __boundcheck_metadata_store((void *)(__tmp_string_40),(void *)(__tmp_string_40+6));
char *__tmp_string_39="and";
 __boundcheck_metadata_store((void *)(__tmp_string_39),(void *)(__tmp_string_39+3));
char *__tmp_string_38="for";
 __boundcheck_metadata_store((void *)(__tmp_string_38),(void *)(__tmp_string_38+3));
char *__tmp_string_37="provide";
 __boundcheck_metadata_store((void *)(__tmp_string_37),(void *)(__tmp_string_37+7));
char *__tmp_string_36="termed";
 __boundcheck_metadata_store((void *)(__tmp_string_36),(void *)(__tmp_string_36+6));
char *__tmp_string_35="free";
 __boundcheck_metadata_store((void *)(__tmp_string_35),(void *)(__tmp_string_35+4));
char *__tmp_string_34="normal";
 __boundcheck_metadata_store((void *)(__tmp_string_34),(void *)(__tmp_string_34+6));
char *__tmp_string_33="wavez";
 __boundcheck_metadata_store((void *)(__tmp_string_33),(void *)(__tmp_string_33+5));
char *__tmp_string_32="conductor";
 __boundcheck_metadata_store((void *)(__tmp_string_32),(void *)(__tmp_string_32+9));
char *__tmp_string_31="phase";
 __boundcheck_metadata_store((void *)(__tmp_string_31),(void *)(__tmp_string_31+5));
char *__tmp_string_30="appears";
 __boundcheck_metadata_store((void *)(__tmp_string_30),(void *)(__tmp_string_30+7));
char *__tmp_string_29="images";
 __boundcheck_metadata_store((void *)(__tmp_string_29),(void *)(__tmp_string_29+6));
char *__tmp_string_28="phase";
 __boundcheck_metadata_store((void *)(__tmp_string_28),(void *)(__tmp_string_28+5));
char *__tmp_string_27="current";
 __boundcheck_metadata_store((void *)(__tmp_string_27),(void *)(__tmp_string_27+7));
char *__tmp_string_26="freq";
 __boundcheck_metadata_store((void *)(__tmp_string_26),(void *)(__tmp_string_26+4));
char *__tmp_string_25="loom";
 __boundcheck_metadata_store((void *)(__tmp_string_25),(void *)(__tmp_string_25+4));
char *__tmp_string_24="doom";
 __boundcheck_metadata_store((void *)(__tmp_string_24),(void *)(__tmp_string_24+4));
char *__tmp_string_23="magnet";
 __boundcheck_metadata_store((void *)(__tmp_string_23),(void *)(__tmp_string_23+6));
char *__tmp_string_22="texture";
 __boundcheck_metadata_store((void *)(__tmp_string_22),(void *)(__tmp_string_22+7));
char *__tmp_string_21="zoom";
 __boundcheck_metadata_store((void *)(__tmp_string_21),(void *)(__tmp_string_21+4));
char *__tmp_string_20="and";
 __boundcheck_metadata_store((void *)(__tmp_string_20),(void *)(__tmp_string_20+3));
char *__tmp_string_19="impact";
 __boundcheck_metadata_store((void *)(__tmp_string_19),(void *)(__tmp_string_19+6));
char *__tmp_string_18="that";
 __boundcheck_metadata_store((void *)(__tmp_string_18),(void *)(__tmp_string_18+4));
char *__tmp_string_17="boom";
 __boundcheck_metadata_store((void *)(__tmp_string_17),(void *)(__tmp_string_17+4));
char *__tmp_string_16="regime";
 __boundcheck_metadata_store((void *)(__tmp_string_16),(void *)(__tmp_string_16+6));
char *__tmp_string_15="row";
 __boundcheck_metadata_store((void *)(__tmp_string_15),(void *)(__tmp_string_15+3));
char *__tmp_string_14="new";
 __boundcheck_metadata_store((void *)(__tmp_string_14),(void *)(__tmp_string_14+3));
char *__tmp_string_13="field";
 __boundcheck_metadata_store((void *)(__tmp_string_13),(void *)(__tmp_string_13+5));
char *__tmp_string_12="x";
 __boundcheck_metadata_store((void *)(__tmp_string_12),(void *)(__tmp_string_12+1));
char *__tmp_string_11="x";
 __boundcheck_metadata_store((void *)(__tmp_string_11),(void *)(__tmp_string_11+1));
char *__tmp_string_10="xx";
 __boundcheck_metadata_store((void *)(__tmp_string_10),(void *)(__tmp_string_10+2));
char *__tmp_string_9="xx";
 __boundcheck_metadata_store((void *)(__tmp_string_9),(void *)(__tmp_string_9+2));
char *__tmp_string_8="oh";
 __boundcheck_metadata_store((void *)(__tmp_string_8),(void *)(__tmp_string_8+2));
char *__tmp_string_7="oo";
 __boundcheck_metadata_store((void *)(__tmp_string_7),(void *)(__tmp_string_7+2));
char *__tmp_string_6="hoo";
 __boundcheck_metadata_store((void *)(__tmp_string_6),(void *)(__tmp_string_6+3));
char *__tmp_string_5="yoo";
 __boundcheck_metadata_store((void *)(__tmp_string_5),(void *)(__tmp_string_5+3));
char *__tmp_string_4="dad";
 __boundcheck_metadata_store((void *)(__tmp_string_4),(void *)(__tmp_string_4+3));
char *__tmp_string_3="it";
 __boundcheck_metadata_store((void *)(__tmp_string_3),(void *)(__tmp_string_3+2));
char *__tmp_string_2="not";
 __boundcheck_metadata_store((void *)(__tmp_string_2),(void *)(__tmp_string_2+3));
char *__tmp_string_1="you";
 __boundcheck_metadata_store((void *)(__tmp_string_1),(void *)(__tmp_string_1+3));
char *__tmp_string_0="abb";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+3));
char *find_strings[] = {__tmp_string_0,  __tmp_string_1, __tmp_string_2, __tmp_string_3, __tmp_string_4, __tmp_string_5, __tmp_string_6,
                              __tmp_string_7, __tmp_string_8, __tmp_string_9, __tmp_string_10, __tmp_string_11, __tmp_string_12, __tmp_string_13, __tmp_string_14, __tmp_string_15,
			      __tmp_string_16, __tmp_string_17, __tmp_string_18, __tmp_string_19, __tmp_string_20, __tmp_string_21, __tmp_string_22,
			      __tmp_string_23, __tmp_string_24, __tmp_string_25, __tmp_string_26, __tmp_string_27, __tmp_string_28,
			      __tmp_string_29, 
			      __tmp_string_30, __tmp_string_31, __tmp_string_32, __tmp_string_33, 
			      __tmp_string_34, __tmp_string_35, __tmp_string_36,
			      __tmp_string_37, __tmp_string_38, __tmp_string_39, __tmp_string_40, __tmp_string_41, __tmp_string_42,
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
			      NULL};
__boundcheck_metadata_store((void *)(&find_strings),(void *)((size_t)(&find_strings)+sizeof(find_strings)*8-1));

      char *__tmp_string_113="principles.";
 __boundcheck_metadata_store((void *)(__tmp_string_113),(void *)(__tmp_string_113+11));
char *__tmp_string_112="whether and how to implement such";
 __boundcheck_metadata_store((void *)(__tmp_string_112),(void *)(__tmp_string_112+33));
char *__tmp_string_111="Executive Officers and I will then decide";
 __boundcheck_metadata_store((void *)(__tmp_string_111),(void *)(__tmp_string_111+41));
char *__tmp_string_110="in this regard. The University's";
 __boundcheck_metadata_store((void *)(__tmp_string_110),(void *)(__tmp_string_110+32));
char *__tmp_string_109="and it will recommend guiding principles";
 __boundcheck_metadata_store((void *)(__tmp_string_109),(void *)(__tmp_string_109+40));
char *__tmp_string_108="be associated with such organizations";
 __boundcheck_metadata_store((void *)(__tmp_string_108),(void *)(__tmp_string_108+37));
char *__tmp_string_107="administrators and faculty members should";
 __boundcheck_metadata_store((void *)(__tmp_string_107),(void *)(__tmp_string_107+41));
char *__tmp_string_106="what ways the University, its";
 __boundcheck_metadata_store((void *)(__tmp_string_106),(void *)(__tmp_string_106+29));
char *__tmp_string_105="consider under what circumstances and in";
 __boundcheck_metadata_store((void *)(__tmp_string_105),(void *)(__tmp_string_105+40));
char *__tmp_string_104="question of privileged space also will";
 __boundcheck_metadata_store((void *)(__tmp_string_104),(void *)(__tmp_string_104+38));
char *__tmp_string_103="February 25, 2000 that is considering the";
 __boundcheck_metadata_store((void *)(__tmp_string_103),(void *)(__tmp_string_103+41));
char *__tmp_string_102="the panel described in my Statement of";
 __boundcheck_metadata_store((void *)(__tmp_string_102),(void *)(__tmp_string_102+38));
char *__tmp_string_101="with student organizations. Accordingly";
 __boundcheck_metadata_store((void *)(__tmp_string_101),(void *)(__tmp_string_101+39));
char *__tmp_string_100="and scope of University involvement";
 __boundcheck_metadata_store((void *)(__tmp_string_100),(void *)(__tmp_string_100+35));
char *__tmp_string_99="important question as to the proper nature";
 __boundcheck_metadata_store((void *)(__tmp_string_99),(void *)(__tmp_string_99+42));
char *__tmp_string_98="student organization have raised an";
 __boundcheck_metadata_store((void *)(__tmp_string_98),(void *)(__tmp_string_98+35));
char *__tmp_string_97="The recent protests about the Michigamua";
 __boundcheck_metadata_store((void *)(__tmp_string_97),(void *)(__tmp_string_97+40));
char *__tmp_string_96="electromagnetic structures.";
 __boundcheck_metadata_store((void *)(__tmp_string_96),(void *)(__tmp_string_96+27));
char *__tmp_string_95="low-profile antennas and other";
 __boundcheck_metadata_store((void *)(__tmp_string_95),(void *)(__tmp_string_95+30));
char *__tmp_string_94="a useful new ground plane for novel";
 __boundcheck_metadata_store((void *)(__tmp_string_94),(void *)(__tmp_string_94+35));
char *__tmp_string_93="high-impedance surface, provides";
 __boundcheck_metadata_store((void *)(__tmp_string_93),(void *)(__tmp_string_93+32));
char *__tmp_string_92="space. This new material, termed a";
 __boundcheck_metadata_store((void *)(__tmp_string_92),(void *)(__tmp_string_92+34));
char *__tmp_string_91="radiate efficiently into free";
 __boundcheck_metadata_store((void *)(__tmp_string_91),(void *)(__tmp_string_91+29));
char *__tmp_string_90="waves do not propagate, and instead";
 __boundcheck_metadata_store((void *)(__tmp_string_90),(void *)(__tmp_string_90+35));
char *__tmp_string_89="conductors. Furthermore, surface";
 __boundcheck_metadata_store((void *)(__tmp_string_89),(void *)(__tmp_string_89+32));
char *__tmp_string_88="out-of-phase as they are on normal";
 __boundcheck_metadata_store((void *)(__tmp_string_88),(void *)(__tmp_string_88+34));
char *__tmp_string_87="appear in-phase, rather than";
 __boundcheck_metadata_store((void *)(__tmp_string_87),(void *)(__tmp_string_87+28));
char *__tmp_string_86="waves, and the effective image currents";
 __boundcheck_metadata_store((void *)(__tmp_string_86),(void *)(__tmp_string_86+39));
char *__tmp_string_85="reverse the phase of reflected";
 __boundcheck_metadata_store((void *)(__tmp_string_85),(void *)(__tmp_string_85+30));
char *__tmp_string_84="not conduct AC currents. It does not";
 __boundcheck_metadata_store((void *)(__tmp_string_84),(void *)(__tmp_string_84+36));
char *__tmp_string_83="particular frequency range it does";
 __boundcheck_metadata_store((void *)(__tmp_string_83),(void *)(__tmp_string_83+34));
char *__tmp_string_82="conducts DC currents, but over a";
 __boundcheck_metadata_store((void *)(__tmp_string_82),(void *)(__tmp_string_82+32));
char *__tmp_string_81="metal, the structure";
 __boundcheck_metadata_store((void *)(__tmp_string_81),(void *)(__tmp_string_81+20));
char *__tmp_string_80="electromagnetic properties. Made of solid";
 __boundcheck_metadata_store((void *)(__tmp_string_80),(void *)(__tmp_string_80+41));
char *__tmp_string_79="special texture which alters its";
 __boundcheck_metadata_store((void *)(__tmp_string_79),(void *)(__tmp_string_79+32));
char *__tmp_string_78="conductive surface, covered with a";
 __boundcheck_metadata_store((void *)(__tmp_string_78),(void *)(__tmp_string_78+34));
char *__tmp_string_77="antennas. It consists of a";
 __boundcheck_metadata_store((void *)(__tmp_string_77),(void *)(__tmp_string_77+26));
char *__tmp_string_76="significant impact on the field of";
 __boundcheck_metadata_store((void *)(__tmp_string_76),(void *)(__tmp_string_76+34));
char *__tmp_string_75="developed that is having a";
 __boundcheck_metadata_store((void *)(__tmp_string_75),(void *)(__tmp_string_75+26));
char *__tmp_string_74="electromagnetic crystal has been";
 __boundcheck_metadata_store((void *)(__tmp_string_74),(void *)(__tmp_string_74+32));
char *__tmp_string_73="regime. A new type of metallic";
 __boundcheck_metadata_store((void *)(__tmp_string_73),(void *)(__tmp_string_73+30));
char *__tmp_string_72="applications in the RF and microwave";
 __boundcheck_metadata_store((void *)(__tmp_string_72),(void *)(__tmp_string_72+36));
char *__tmp_string_71="crystals has found new";
 __boundcheck_metadata_store((void *)(__tmp_string_71),(void *)(__tmp_string_71+22));
char *__tmp_string_70="In recent years, the field of photonic ";
 __boundcheck_metadata_store((void *)(__tmp_string_70),(void *)(__tmp_string_70+39));
char *__tmp_string_69=".";
 __boundcheck_metadata_store((void *)(__tmp_string_69),(void *)(__tmp_string_69+1));
char *__tmp_string_68="x";
 __boundcheck_metadata_store((void *)(__tmp_string_68),(void *)(__tmp_string_68+1));
char *__tmp_string_67="xx";
 __boundcheck_metadata_store((void *)(__tmp_string_67),(void *)(__tmp_string_67+2));
char *__tmp_string_66="yoohoo";
 __boundcheck_metadata_store((void *)(__tmp_string_66),(void *)(__tmp_string_66+6));
char *__tmp_string_65="yoohoo";
 __boundcheck_metadata_store((void *)(__tmp_string_65),(void *)(__tmp_string_65+6));
char *__tmp_string_64="yoohoo";
 __boundcheck_metadata_store((void *)(__tmp_string_64),(void *)(__tmp_string_64+6));
char *__tmp_string_63="yoohoo";
 __boundcheck_metadata_store((void *)(__tmp_string_63),(void *)(__tmp_string_63+6));
char *__tmp_string_62="yoohoo";
 __boundcheck_metadata_store((void *)(__tmp_string_62),(void *)(__tmp_string_62+6));
char *__tmp_string_61="hodad";
 __boundcheck_metadata_store((void *)(__tmp_string_61),(void *)(__tmp_string_61+5));
char *__tmp_string_60="But it is here";
 __boundcheck_metadata_store((void *)(__tmp_string_60),(void *)(__tmp_string_60+14));
char *__tmp_string_59="It isn't here";
 __boundcheck_metadata_store((void *)(__tmp_string_59),(void *)(__tmp_string_59+13));
char *__tmp_string_58="your";
 __boundcheck_metadata_store((void *)(__tmp_string_58),(void *)(__tmp_string_58+4));
char *__tmp_string_57="cabbie";
 __boundcheck_metadata_store((void *)(__tmp_string_57),(void *)(__tmp_string_57+6));
char *search_strings[] = {__tmp_string_57, __tmp_string_58, __tmp_string_59,
                                __tmp_string_60, __tmp_string_61, __tmp_string_62, __tmp_string_63,
                                __tmp_string_64, __tmp_string_65, __tmp_string_66, __tmp_string_67, __tmp_string_68, __tmp_string_69, 
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
				__tmp_string_113			       
};
__boundcheck_metadata_store((void *)(&search_strings),(void *)((size_t)(&search_strings)+sizeof(search_strings)*8-1));

      int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


      for (i = 0; find_strings[_RV_insert_check(0,58,149,32,"main",i)]; i++)
      {
            init_search(find_strings[_RV_insert_check(0,58,151,38,"main",i)]);
            here = strsearch(search_strings[_RV_insert_check(0,57,152,45,"main",i)]);
            printf("\"%s\" is%s in \"%s\"", find_strings[_RV_insert_check(0,58,153,58,"main",i)],
                  here ? "" : " not", search_strings[_RV_insert_check(0,57,154,54,"main",i)]);
            if (here)
                  printf(" [\"%s\"]", here);
            putchar('\n');
      }

      return 0;
}

