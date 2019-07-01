/*
 * IupScintilla sample
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>


#include <iup.h>
#include "Scintilla.h"
#include <iup_scintilla.h>
#include <iup_config.h>


#include "csc_debug.h"


static const char* sampleCode =
{
"/* Block comment */\n"
"#include<stdio.h>\n#include<iup.h>\n\n"
"void SampleTest() {\n  printf(\"Printing float: %f\\n\", 12.5);\n}\n\n"
"void SampleTest2() {\n  printf(\"Printing char: %c\\n\", 'c');\n}\n\n"
"int main(int argc, char **argv) {\n"
"  // Start up IUP\n"
"  IupOpen(&argc, &argv);\n"
"  IupSetGlobal(\"SINGLEINSTANCE\", \"Iup Sample\");\n\n"
"  if(!IupGetGlobal(\"SINGLEINSTANCE\")) {\n"
"    IupClose(); \n"
"    return EXIT_SUCCESS; \n  }\n\n"
"  SampleTest();\n"
"  SampleTest2();\n"
"  printf(\"Printing an integer: %d\\n\", 37);\n\n"
"  IupMainLoop();\n"
"  IupClose();\n"
"  return EXIT_SUCCESS;\n}\n"
};


static Ihandle * handle_sci = NULL;


static int k_any(Ihandle *ih, int c)
{
	//  printf("K_ANY(key: %d)\n", c);
	if (c == K_cS) {return IUP_IGNORE;}
	if (c == K_cO)
	{
		IupSetAttribute(ih, "OVERWRITE", "ON");
		return IUP_IGNORE;
	}
	return IUP_CONTINUE;
}

static int marginclick_cb(Ihandle *self, int margin, int line, char* status)
{
	//printf("MARGINCLICK_CB(Margin: %d, Line: %d, Status:%s)\n", margin, line, status);
	//printf("Fold Level = %s\n", IupGetAttributeId(self, "FOLDLEVEL", line));
	if (margin == 1)
	{
		IupSetfAttribute (self, "FOLDTOGGLE", "%d", line);
	}

	if (margin == 2)
	{
		long int value = IupGetIntId(self, "MARKERGET", line);
		printf ("MARKERGET %x\n", value);
		if (value==0){IupSetIntId(self, "MARKERADD", line, margin);}
		else{IupSetIntId(self, "MARKERDELETE", line, margin);}

	}


	return IUP_DEFAULT;
}

static int hotspotclick_cb(Ihandle *self, int pos, int line, int col, char* status)
{
	char *text = IupGetAttributeId(self, "LINE", line);
	printf("HOTSPOTCLICK_CB (Pos: %d, Line: %d, Col: %d, Status:%s)\n", pos, line, col, status);
	printf("    line text = %s\n", text);
	return IUP_DEFAULT;
}

static int button_cb(Ihandle* self, int button, int pressed, int x, int y, char* status)
{
	printf("BUTTON_CB = button: %d, pressed: %d, x: %d, y: %d, status: %s\n", button, pressed, x, y, status);
	(void)self;
	return IUP_DEFAULT;
}

static int motion_cb(Ihandle *self, int x, int y, char *status)
{
	//printf("MOTION_CB = x: %d, y: %d, status: %s\n", x, y, status);
	(void)self;
	return IUP_DEFAULT;
}

static int caret_cb(Ihandle *self, int lin, int col, int pos)
{
	printf("CARET_CB = lin: %d, col: %d, pos: %d\n", lin, col, pos);
	(void)self;
	return IUP_DEFAULT;
}

static int valuechanged_cb(Ihandle *self)
{
	printf("VALUECHANGED_CB\n");
	(void)self;
	return IUP_DEFAULT;
}

static int action_cb(Ihandle *self, int insert, int pos, int length, char* text)
{
	printf("ACTION = insert: %d, pos: %d, lenght:%d, text: %s\n", insert, pos, length, text);
	(void)self;
	return IUP_IGNORE;
}

static void set_attribs (Ihandle *sci)
{
	IupSetAttribute(sci, "CLEARALL", "");
	IupSetAttribute(sci, "LEXERLANGUAGE", "cpp");
	IupSetAttribute(sci, "KEYWORDS0", "void struct union enum char short int long double float signed unsigned const static extern auto register volatile bool class private protected public friend inline template virtual asm explicit typename mutable"
	"if else switch case default break goto return for while do continue typedef sizeof NULL new delete throw try catch namespace operator this const_cast static_cast dynamic_cast reinterpret_cast true false using"
	"typeid and and_eq bitand bitor compl not not_eq or or_eq xor xor_eq");
	//IupSetAttribute(sci, "STYLEFONT32", "Courier New");
	IupSetAttribute(sci, "STYLEFONT32", "Consolas");
	IupSetAttribute(sci, "STYLEFONTSIZE32", "11");
	IupSetAttribute(sci, "STYLECLEARALL", "Yes");  /* sets all styles to have the same attributes as 32 */
	IupSetAttribute(sci, "STYLEFGCOLOR1", "0 128 0");    // 1-C comment
	IupSetAttribute(sci, "STYLEFGCOLOR2", "0 128 0");    // 2-C++ comment line
	IupSetAttribute(sci, "STYLEFGCOLOR4", "128 0 0");    // 4-Number
	IupSetAttribute(sci, "STYLEFGCOLOR5", "0 0 255");    // 5-Keyword
	IupSetAttribute(sci, "STYLEFGCOLOR6", "160 20 20");  // 6-String
	IupSetAttribute(sci, "STYLEFGCOLOR7", "128 0 0");    // 7-Character
	IupSetAttribute(sci, "STYLEFGCOLOR9", "0 0 255");    // 9-Preprocessor block
	IupSetAttribute(sci, "STYLEFGCOLOR10", "255 0 255"); // 10-Operator
	IupSetAttribute(sci, "STYLEBOLD10", "YES");
	// 11-Identifier
	IupSetAttribute(sci, "STYLEHOTSPOT6", "YES");
	IupSetAttribute(sci, "INSERT0", sampleCode);
	//IupSetAttribute(sci, "INSERT-1", "           ");
	//if (IupGetInt(NULL, "UTF8MODE"))
	//  IupSetAttribute(sci, "PREPEND", "//  UTF8MODE Enabled: (Ã§Ã£ÃµÃ¡Ã³Ã©)");
	//else
	//  IupSetAttribute(sci, "PREPEND", "//  UTF8MODE Disabled: (çãõáóé)");
	IupSetAttribute(sci, "MARGINWIDTH0", "50");

	if (1)
	{
		IupSetAttribute(sci, "PROPERTY", "fold=1");
		IupSetAttribute(sci, "PROPERTY", "fold.compact=0");
		IupSetAttribute(sci, "PROPERTY", "fold.comment=1");
		IupSetAttribute(sci, "PROPERTY", "fold.preprocessor=1");
		IupSetAttribute(sci, "MARGINWIDTH1", "20");
		IupSetAttribute(sci, "MARGINTYPE1",  "SYMBOL");
		IupSetAttribute(sci, "MARGINMASKFOLDERS1",  "Yes");
		IupSetAttribute(sci, "MARKERDEFINE", "FOLDER=PLUS");
		IupSetAttribute(sci, "MARKERDEFINE", "FOLDEROPEN=MINUS");
		IupSetAttribute(sci, "MARKERDEFINE", "FOLDEREND=EMPTY");
		IupSetAttribute(sci, "MARKERDEFINE", "FOLDERMIDTAIL=EMPTY");
		IupSetAttribute(sci, "MARKERDEFINE", "FOLDEROPENMID=EMPTY");
		IupSetAttribute(sci, "MARKERDEFINE", "FOLDERSUB=EMPTY");
		IupSetAttribute(sci, "MARKERDEFINE", "FOLDERTAIL=EMPTY");
		IupSetAttribute(sci, "FOLDFLAGS", "LINEAFTER_CONTRACTED");
		IupSetAttribute(sci, "MARGINSENSITIVE1", "YES");


	}

	if (1)
	{
		//https://www.scintilla.org/ScintillaDox.html
		//https://qscintilla.com/symbol-margin/
		IupSetIntId(sci, "MARGINMASK", 2, 0x000005);
		IupSetAttributeId(sci, "MARKERFGCOLOR", 2, "255 0 0");
		IupSetAttributeId(sci, "MARKERBGCOLOR", 2, "255 0 0");
		IupSetAttributeId(sci, "MARKERALPHA", 2, "80");
		IupSetAttributeId(sci, "MARKERSYMBOL", 2, "CIRCLE");
		IupSetAttributeId (sci, "MARGINWIDTH", 2, "20");
		IupSetAttributeId (sci, "MARGINSENSITIVE", 2, "YES");
	}

	//IupSetAttribute(sci, "SELECTION", "0,2:1,10");
	//printf("SELECTION=\"%s\"\n", IupGetAttribute(sci, "SELECTION"));
	printf("strlen=%zu\n", strlen(sampleCode));
	printf("COUNT=%s\n", IupGetAttribute(sci, "COUNT"));
	//printf("VALUE=\"%s\"\n", IupGetAttribute(sci, "VALUE"));
	printf("LINECOUNT=%s\n", IupGetAttribute(sci, "LINECOUNT"));
	printf("LINEVALUE=\"%s\"\n", IupGetAttribute(sci, "LINEVALUE"));
}

void ScintillaTest(void)
{
	Ihandle *dlg;
	IupScintillaOpen();
	IupSetGlobal("UTF8MODE", "No");
	// Creates an instance of the Scintilla control
	handle_sci = IupScintilla();
	//  IupSetAttribute(sci, "VISIBLECOLUMNS", "80");
	//  IupSetAttribute(sci, "VISIBLELINES", "40");
	//IupSetAttribute(sci, "SCROLLBAR", "NO");
	//  IupSetAttribute(sci, "BORDER", "NO");
	IupSetAttribute(handle_sci, "EXPAND", "Yes");
	//  IupSetAttribute(sci, "OVERWRITE", "ON");
	IupSetCallback(handle_sci, "MARGINCLICK_CB", (Icallback)marginclick_cb);
	IupSetCallback(handle_sci, "HOTSPOTCLICK_CB", (Icallback)hotspotclick_cb);
	IupSetCallback(handle_sci, "BUTTON_CB", (Icallback)button_cb);
	IupSetCallback(handle_sci, "MOTION_CB", (Icallback)motion_cb);
	IupSetCallback(handle_sci, "K_ANY", (Icallback)k_any);
	IupSetCallback(handle_sci, "CARET_CB", (Icallback)caret_cb);
	IupSetCallback(handle_sci, "VALUECHANGED_CB", (Icallback)valuechanged_cb);
	IupSetCallback(handle_sci, "ACTION", (Icallback)action_cb);
	// Creates a dialog containing the control

	dlg = IupDialog(IupVbox(handle_sci, NULL));
	IupSetAttribute(dlg, "TITLE", "IupScintilla");
	IupSetAttribute(dlg, "RASTERSIZE", "700x500");
	IupSetAttribute(dlg, "MARGIN", "10x10");
	// Shows dialog
	IupShow(dlg);
	IupSetAttribute(dlg, "RASTERSIZE", NULL);
	set_attribs(handle_sci);
}

static int btn_open_action (Ihandle* ih)
{
	printf ("btn_open_action!\n");
	ScintillaTest();
}

static int btn_next_action (Ihandle* ih)
{
	printf ("btn_next_action!\n");
	int lin = 9;
	int pos1;
	int pos2;
	IupTextConvertLinColToPos(handle_sci, lin, 0, &pos1);
	char * text = IupGetAttributeId(handle_sci, "LINE", lin);
	int len = (int)strlen(text);
	printf ("len %i\n", len);
	IupSetAttribute(handle_sci, "INDICATORSTYLE0", "FULLBOX");
	//IupScintillaSendMessage (handle_sci, SCI_SETINDICATORCURRENT, 4, 0);
	IupSetStrf(handle_sci, "INDICATORFILLRANGE", "%d:%d", pos1, len);
	//IupScintillaSendMessage(handle_sci, SCI_GETCOLUMN, pMsg->position, 0);
	IupShow (IupElementPropertiesDialog (handle_sci));
	//IupScintillaSendMessage(handle_sci, SCI_SETMARGINS, 4, 0);
}

void test ()
{
	Ihandle * btn_open = IupButton ("open", NULL);
	Ihandle * btn_next = IupButton ("next", NULL);
	IupSetCallback(btn_open, "ACTION", (Icallback)btn_open_action);
	IupSetCallback(btn_next, "ACTION", (Icallback)btn_next_action);
	Ihandle * dlg = IupDialog(IupVbox(btn_open, btn_next, NULL));
	IupSetAttribute(dlg, "TITLE", "IupScintilla");
	IupSetAttribute(dlg, "RASTERSIZE", "700x500");
	IupSetAttribute(dlg, "MARGIN", "10x10");
	// Shows dialog
	IupShow(dlg);
	IupSetAttribute(dlg, "RASTERSIZE", NULL);

}

int main(int argc, char* argv[])
{
	setbuf (stdout, NULL);
	IupOpen(&argc, &argv);
	test ();
	IupMainLoop();
	IupClose();
	return EXIT_SUCCESS;
}

