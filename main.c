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

enum app_scimargin
{
	APP_SCIMARGIN_LINES,
	APP_SCIMARGIN_BM0,
	APP_SCIMARGIN_BM1,
	APP_SCIMARGIN_BM2,
	APP_SCIMARGIN_FOLD,
	APP_SCIMARGIN_TEXT
};

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


static int k_any (Ihandle *ih, int c)
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

static int marginclick_cb (Ihandle *self, int margin, int line, char* status)
{
	//printf("MARGINCLICK_CB(Margin: %d, Line: %d, Status:%s)\n", margin, line, status);
	//printf("Fold Level = %s\n", IupGetAttributeId(self, "FOLDLEVEL", line));
	int mark = IupGetIntId (self, "MARKERGET", line);
	//printf ("MARKERGET %x\n", mark);

	if (margin == APP_SCIMARGIN_FOLD)
	{
		IupSetfAttribute (self, "FOLDTOGGLE", "%d", line);
	}

	if (margin == APP_SCIMARGIN_BM0)
	{
		if (mark & (1 << 4)) {IupSetIntId (self, "MARKERDELETE", line, 4);}
		else {IupSetIntId (self, "MARKERADD", line, 4);}
	}

	if (margin == APP_SCIMARGIN_BM1)
	{
		if (mark & (1 << 5)) {IupSetIntId (self, "MARKERDELETE", line, 5);}
		else {IupSetIntId (self, "MARKERADD", line, 5);}
	}

	if (margin == APP_SCIMARGIN_BM2)
	{
		if (mark & (1 << 6)) {IupSetIntId (self, "MARKERDELETE", line, 6);}
		else {IupSetIntId (self, "MARKERADD", line, 6);}
	}

	return IUP_DEFAULT;
}

static int hotspotclick_cb (Ihandle *self, int pos, int line, int col, char* status)
{
	char *text = IupGetAttributeId(self, "LINE", line);
	printf("HOTSPOTCLICK_CB (Pos: %d, Line: %d, Col: %d, Status:%s)\n", pos, line, col, status);
	printf("    line text = %s\n", text);
	return IUP_DEFAULT;
}

static int button_cb (Ihandle* self, int button, int pressed, int x, int y, char* status)
{
	printf("BUTTON_CB = button: %d, pressed: %d, x: %d, y: %d, status: %s\n", button, pressed, x, y, status);
	(void)self;
	return IUP_DEFAULT;
}

/*
static int motion_cb (Ihandle *self, int x, int y, char *status)
{
	//printf("MOTION_CB = x: %d, y: %d, status: %s\n", x, y, status);
	(void)self;
	return IUP_DEFAULT;
}
*/

static int caret_cb (Ihandle *self, int lin, int col, int pos)
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
	IupSetAttribute(sci, "STYLEHOTSPOT6", "YES");
	IupSetAttribute(sci, "INSERT0", sampleCode);
	IupSetAttribute(sci, "TABSIZE", "4");
	IupSetAttribute(sci, "WHITESPACEVIEW", "VISIBLEALWAYS");
	IupSetAttribute(sci, "WHITESPACEFGCOLOR", "200 200 200");

	IupSetAttributeId (sci, "MARGINWIDTH", APP_SCIMARGIN_LINES, "50");
	IupSetAttributeId (sci, "MARGINWIDTH", APP_SCIMARGIN_BM0, "20");
	IupSetAttributeId (sci, "MARGINWIDTH", APP_SCIMARGIN_BM1, "20");
	IupSetAttributeId (sci, "MARGINWIDTH", APP_SCIMARGIN_BM2, "20");
	IupSetAttributeId (sci, "MARGINWIDTH", APP_SCIMARGIN_FOLD, "20");
	IupSetAttributeId (sci, "MARGINSENSITIVE", APP_SCIMARGIN_LINES, "YES");
	IupSetAttributeId (sci, "MARGINSENSITIVE", APP_SCIMARGIN_BM0, "YES");
	IupSetAttributeId (sci, "MARGINSENSITIVE", APP_SCIMARGIN_BM1, "YES");
	IupSetAttributeId (sci, "MARGINSENSITIVE", APP_SCIMARGIN_BM2, "YES");
	IupSetAttributeId (sci, "MARGINSENSITIVE", APP_SCIMARGIN_FOLD, "YES");

	//https://www.scintilla.org/ScintillaDoc.html#SCI_MARKERDEFINE
	IupSetAttributeId (sci, "MARKERSYMBOL", 4, "CIRCLE");
	IupSetAttributeId (sci, "MARKERSYMBOL", 5, "ROUNDRECT");
	IupSetAttributeId (sci, "MARKERSYMBOL", 6, "ARROW");
	IupSetAttributeId (sci, "MARKERSYMBOL", 7, "SHORTARROW");
	IupSetAttributeId (sci, "MARKERSYMBOL", 8, "SC_MARK_BACKGROUND");

	IupSetIntId(sci, "MARGINMASK", APP_SCIMARGIN_BM0, 1 << 4);
	IupSetIntId(sci, "MARGINMASK", APP_SCIMARGIN_BM1, 1 << 5);
	IupSetIntId(sci, "MARGINMASK", APP_SCIMARGIN_BM2, 1 << 6);
	IupSetIntId(sci, "MARGINMASK", APP_SCIMARGIN_FOLD, 1 << 7);
	IupSetIntId(sci, "MARGINMASK", APP_SCIMARGIN_TEXT, 1 << 8);

	IupSetAttributeId(sci, "MARKERBGCOLOR", 8, "255 0 0");
	IupSetAttributeId(sci, "MARKERALPHA", 8, "80");

	IupSetAttribute(sci, "PROPERTY", "fold=1");
	IupSetAttribute(sci, "PROPERTY", "fold.compact=0");
	IupSetAttribute(sci, "PROPERTY", "fold.comment=1");
	IupSetAttribute(sci, "PROPERTY", "fold.preprocessor=1");

	IupSetAttributeId (sci, "MARGINTYPE", APP_SCIMARGIN_FOLD, "SYMBOL");
	IupSetAttributeId (sci, "MARGINMASKFOLDERS", APP_SCIMARGIN_FOLD, "Yes");

	IupSetAttribute(sci, "MARKERDEFINE", "FOLDER=PLUS");
	IupSetAttribute(sci, "MARKERDEFINE", "FOLDEROPEN=MINUS");
	IupSetAttribute(sci, "MARKERDEFINE", "FOLDEREND=EMPTY");
	IupSetAttribute(sci, "MARKERDEFINE", "FOLDERMIDTAIL=EMPTY");
	IupSetAttribute(sci, "MARKERDEFINE", "FOLDEROPENMID=EMPTY");
	IupSetAttribute(sci, "MARKERDEFINE", "FOLDERSUB=EMPTY");
	IupSetAttribute(sci, "MARKERDEFINE", "FOLDERTAIL=EMPTY");
	IupSetAttribute(sci, "FOLDFLAGS", "LINEAFTER_CONTRACTED");

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
	//IupSetCallback(handle_sci, "MOTION_CB", (Icallback)motion_cb);
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

void IupTextConvertLinColToPosLen(Ihandle* ih, int lin, int col, int *pos, int *len)
{
	IupTextConvertLinColToPos (ih, lin, col, pos);
	char * text = IupGetAttributeId (ih, "LINE", lin);
	//assert (text);
	if (text)
	{
		*len = (int)strlen (text); //Maybe use strnlen?
		//printf ("%d:%d\n", *pos, *len);
	}
	else
	{
		*len = 0;
	}
}

static int btn_next_action (Ihandle* ih)
{
	printf ("btn_next_action!\n");
	/*
	static int lin = 0;
	int pos;
	int len;
	IupTextConvertLinColToPosLen (handle_sci, lin, 0, &pos, &len);
	IupSetStrf (handle_sci, "INDICATORCLEARRANGE", "%d:%d", pos, len);
	lin ++;
	IupTextConvertLinColToPosLen (handle_sci, lin, 0, &pos, &len);
	IupSetStrf (handle_sci, "INDICATORFILLRANGE", "%d:%d", pos, len);
	*/
	static int line = 0;
	IupSetIntId (handle_sci, "MARKERDELETE", line, 8);
	line ++;
	IupSetIntId (handle_sci, "MARKERADD", line, 8);
}

static int btn_prop_action (Ihandle* ih)
{
	IupShow (IupElementPropertiesDialog (handle_sci));
}

void test ()
{
	Ihandle * btn_open = IupButton ("open", NULL);
	Ihandle * btn_next = IupButton ("next", NULL);
	Ihandle * btn_prop = IupButton ("IupElementPropertiesDialog", NULL);
	IupSetCallback(btn_open, "ACTION", (Icallback)btn_open_action);
	IupSetCallback(btn_next, "ACTION", (Icallback)btn_next_action);
	IupSetCallback(btn_prop, "ACTION", (Icallback)btn_prop_action);
	Ihandle * dlg = IupDialog(IupVbox(btn_open, btn_next, btn_prop, NULL));
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

