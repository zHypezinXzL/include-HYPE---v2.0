/* * * * * * * * * * * * *( HYPE 2.0 )* * * * * * * * * * * * *\
 *                                                            *
 *          (C) Copyright 2022 by zHypezinXzL (R)             *
 *                                                            *
 * @country  : Brasil                                         *
 * @author   : zHypezinXzL                                    *
 * @github   : https://github.com/zHypezinXzL                 *
 * @date     : 7th September 2022 (Independence Brasil)       *
 * @update   : 11th September 2022                            *
 *                                                            *
 *                    Code changed ( all )                    *
 *                New Function ( HYPE_FSetHex )               *
 *                New Function ( HYPE_FGetHex )               *
 *                                                            *
 *     Time to Drop DOF2 and Dini and start using HYPE        *
 *                                                            *
\* * * * * * * * * * * * *( HYPE 2.0 )* * * * * * * * * * * * */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <a_samp>             /*  Dowload:  https://github.com/zHypezinXzL/include-a_samp.git  */
#include <string>             /*  Dowload:  https://github.com/zHypezinXzL/include-string.git  */
#include <float>              /*  Dowload:  https://github.com/zHypezinXzL/include-float.git   */
#include <file>               /*  Dowload:  https://github.com/zHypezinXzL/include-file.git    */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



/* * * * * * * * * * * * * * * */
#if defined _hype_included
    #endinput
#endif
#define _hype_included
#pragma library hype
/* * * * * * * * * * * * * * * */



/* * * * * * * * * * * * * * * */
#if defined MAX_LINE 
    #define HYPE_MAX_LINE MAX_LINE
#else 
    #define HYPE_MAX_LINE (250)
#endif
/* * * * * * * * * * * * * * * */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
stock HYPE_FCreate(const file[])
{
	new File:fnew = fopen(file, io_write);
	if(fnew) return fclose(fnew);
	return false;
}
stock HYPE_FRemove(const file[])
{
    if(fexist(file)) return fremove(file); 
}
stock HYPE_FExists(const file[])
{
	return fexist(file);
}
stock HYPE_FRename(const oldname[], const newname[])
{
	if(fexist(newname)) return false;
    new File:fnew = fopen(newname, io_write);
    if(fnew) return HYPE_FCopyText(oldname, newname), fclose(fnew), fremove(oldname);
	return false;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FIsSet(const file[], const key[])
{
	new key_size = strlen(key);
	if(key_size == 0 || key_size + 2 > HYPE_MAX_LINE) return false;
	new File:firh, fsec[HYPE_MAX_LINE];
	firh = fopen(file, io_read);
	if(!firh) return false;
	while(fread(firh, fsec))
    {
		if(fsec[key_size] == '=' && !strcmp(fsec, key, true, key_size)) return true, fclose(firh);
	}
	fclose(firh);
	return false;
}
stock HYPE_FUnSet(const file[], const key[])
{
	new key_size = strlen(key);
	if(key_size == 0 || key_size + 2 > HYPE_MAX_LINE) return false;
	new File:firh, File:fiwh, fsec[HYPE_MAX_LINE];
	format(fsec, HYPE_MAX_LINE, "%s.hype", file); fremove(fsec); firh = fopen(file, io_read);
	if(!firh) return false; fiwh = fopen(fsec, io_write);
	if(!fiwh) return false; fclose(firh);
	while(fread(firh, fsec))
	{
		if(fsec[key_size] == '=' && !strcmp(fsec, key, true, key_size)) { } 
		else HYPE_FNewLine(fsec), fwrite(fiwh, fsec); fwrite(fiwh, "\r\n");
	}
	fclose(firh); fclose(fiwh); format(fsec, HYPE_MAX_LINE, "%s.hype", file);
	if(HYPE_FCopyText(fsec, file)) return fremove(fsec);
	return false;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetString(const file[], const key[], const string[])
{
    if(HYPE_FIsSet(file, key)) HYPE_FUnSet(file, key);
	new key_size = strlen(key); key_size = strlen(string);
	if(key_size == 0 || key_size + key_size + 2 > HYPE_MAX_LINE) return false;
	new File:firh, File:fiwh, fsec[HYPE_MAX_LINE], bool:configured = false;
	format(fsec, sizeof fsec, "%s.hype", file); fremove(fsec); firh = fopen(file, io_read);
	if(!firh) return false; fiwh = fopen(fsec, io_write);
	if(!fiwh) return false, fclose(firh);
	while(fread(firh, fsec))
    {
		if(!configured && fsec[key_size] == '=' && !strcmp(fsec, key, true, key_size)) format(fsec, sizeof fsec, "%s=%s", key, string), configured = true;
        else HYPE_FNewLine(fsec);
		fwrite(fiwh, fsec); fwrite(fiwh, "\r\n");
	}
	if(!configured) format(fsec, sizeof fsec, "%s=%s", key, string), fwrite(fiwh, fsec); fwrite(fiwh, "\r\n");
	fclose(firh); fclose(fiwh); format(fsec, sizeof fsec, "%s.hype", file);
	if(HYPE_FCopyText(fsec, file)) return fremove(fsec);
	return false;
}
stock HYPE_FGetString(const file[], const key[])
{
	new fsec[HYPE_MAX_LINE], key_size = strlen(key);
	if(key_size == 0 || key_size + 2 > HYPE_MAX_LINE) return fsec;
	new File:firh = fopen(file, io_read);
	if(!firh) return fsec;
	while(fread(firh, fsec))
    {
		if(fsec[key_size] == '=' && !strcmp(fsec, key, true, key_size)) 
        {
			HYPE_FNewLine(fsec); strmid(fsec, fsec, key_size + 1, strlen(fsec), HYPE_MAX_LINE);
			fclose(firh); return fsec;
		}
	}
	fclose(firh);
	return fsec;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetInt(const file[], const key[], int)
{
    if(HYPE_FIsSet(file, key)) HYPE_FUnSet(file, key);
    new Int[HYPE_MAX_LINE]; format(Int, sizeof Int, "%d", int);
    return HYPE_FSetString(file, key, Int);
}
stock HYPE_FGetInt(const file[], const key[])
{
    return strval(HYPE_FGetString(file, key)); 
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetFloat(const file[], const key[], Float:float)
{
    if(HYPE_FIsSet(file, key)) HYPE_FUnSet(file, key);
    new Float[HYPE_MAX_LINE]; format(Float, sizeof Float, "%f", float);
    return HYPE_FSetString(file, key, Float);
}
stock Float:HYPE_FGetFloat(const file[], const key[])
{
    return floatstr(HYPE_FGetString(file, key));
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetBool(const file[], const key[], bool:bool)
{
    if(HYPE_FIsSet(file, key)) HYPE_FUnSet(file, key);
	if(bool == true) return HYPE_FSetString(file, key, "true");
    else return HYPE_FSetString(file, key, "false");
}
stock bool:HYPE_FGetBool(const file[], const key[])
{
	new bool[50]; format(bool, sizeof bool, "%s", HYPE_FGetString(file, key));
    if(strcmp(bool, "true") == 0) return true;
	else return false;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetHex(const file[], const key[],  const hex[])
{
	if(!HYPE_FValidHex(hex)) return false;
    if(HYPE_FIsSet(file, key)) HYPE_FUnSet(file, key);
	return HYPE_FSetString(file, key, hex);
}
stock HYPE_FGetHex(const file[], const key[])
{
	if(!HYPE_FValidHex(HYPE_FGetString(file, key))) return false;
    return HYPE_FHexToInt(HYPE_FGetString(file, key));
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FHexToInt(string[])  // const return error / no const return sucess / Code by Forum SA-MP
{
	if(string[0] == 0) return false;
	new i = strlen(string), cur = 1, res = 0;
	while(i > 0) 
	{
		if(string[i - 1] < 58) res = res + cur*(string[i - 1] - 48); 
		else res = res + cur*(string[i - 1] - 65 + 10); cur = cur*16;
	    i--;
  	}
  	return res;
} 
stock HYPE_FNewLine(code[])  // const return error / no const return sucess
{
	new size = strlen(code);
	if(code[0] == 0) return ;
	if((code[size-1] == '\n') || (code[size - 1] == '\r'))
    {
		code[size - 1] = 0;
		if(code[0] == 0) return ;
		else if((code[size - 2] == '\n') || (code[size - 2] == '\r'))
        {
            code[size - 2] = 0;
            if(code[0] == 0) return ;
        }
	}
}
stock HYPE_FCopyText(const oldname[], const newname[])
{
	new File:firh = fopen(oldname, io_read), File:fiwh = fopen(newname, io_write);
	if(!firh) return false;
	if(!fiwh) return fclose(firh);
	new fsec[HYPE_MAX_LINE];
	while(fread(firh, fsec))
    {
		HYPE_FNewLine(fsec); format(fsec, sizeof fsec, "%s\r\n", fsec);
		fwrite(fiwh, fsec);
	}
	fclose(firh); fclose(fiwh);
	return true;
}
stock HYPE_FValidHex(const hex[])
{
	if(strlen(hex) != 10) return false;
	new Mid[50], Mtt[50], c = 2; strmid(Mid, hex, 0, 2, sizeof Mid);
    if(strcmp(Mid, "0x", true) == 0)
    {
        while(c < 11)
        {
            strmid(Mtt, hex, c, c+1, sizeof Mtt);
            if(strcmp(Mtt, "0") == 0 || strcmp(Mtt, "1") == 0 || strcmp(Mtt, "2") == 0 || strcmp(Mtt, "3") == 0 ||
               strcmp(Mtt, "4") == 0 || strcmp(Mtt, "5") == 0 || strcmp(Mtt, "6") == 0 || strcmp(Mtt, "7") == 0 ||
               strcmp(Mtt, "8") == 0 || strcmp(Mtt, "9") == 0 || strcmp(Mtt, "A", false) == 0 || strcmp(Mtt, "B", false) == 0 ||
               strcmp(Mtt, "C", false) == 0 || strcmp(Mtt, "D", false) == 0 || strcmp(Mtt, "E", false) == 0 || strcmp(Mtt, "F", false) == 0)
               {
				   c++;
			       if(c == 10) return true; 
			   }
               else return false;
        }
    }
    return false;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * /
native <--=[C]=-->
native HYPE_FCreate(const file[]);                                          // FC

native <--=[E]=-->
native HYPE_FExists(const file[]);                                          // FE

native <--=[G]=-->
native HYPE_FGetBool(const file[], const key[]);                            // FGB
native HYPE_FGetFloat(const file[], const key[]);                           // FGF
native HYPE_FGetHex(const file[], const key[]);                             // FGH ( New )
native HYPE_FGetInt(const file[], const key[]);                             // FGI
native HYPE_FGetString(const file[], const key[]);                          // FGS

native <--=[I]=-->
native HYPE_FIsSet(const file[], cosnt key[]);                              // FI

native <--=[S]=-->
native HYPE_FSetBool(const file[], const key[], bool:bool);                 // FSB
native HYPE_FSetFloat(const file[], const key[], Float:float);              // FSF
native HYPE_FSetHex(const file[], const key, const hex[]);                  // FSH ( New )
native HYPE_FSetInt(const file[], const key[], int);                        // FSI
native HYPE_FSetString(const file[], const key[], const str[]);             // FSS

native <--=[R]=-->
native HYPE_FRemove(const file[]);                                          // FRM
native HYPE_FRename(const file[]);                                          // FRN

native <--=[U]=-->
native HYPE_FUnSet(const file[], const key[]);                              // FU

native <--=[V]=-->
native HYPE_FValidHex(const hex[]);											// FV
/ * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */     
