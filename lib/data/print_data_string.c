// Helper for autogenerate charset_relation table
#include <windows.h>
#include <winnls.h>
//#include <wine/unicode.h>

#include <stdlib.h>
#include <stdio.h>

#include "charset_names.h"

static const char * get_charset (UINT codepage)
{
  int i;
  for (i = 0; i < sizeof (charset_names) / sizeof (charset_names[0]); i++)
    {
      if (charset_names[i].codepage == codepage)
	{
	  return charset_names[i].charset_name;
	}
    }
	fprintf(stderr,"Can't locate codepage %d for %s locale\n",codepage,getenv("LANG"));
  return "";
}


void print_w( char *str, int w)
{
  char buf[100]; int i;
  sprintf (buf,"%s%s", str, ( w ? ", " : ""));
  printf (buf);
  i=w-strlen(buf);
  if (i>0)
	  for (;i;i--)
  		printf(" ");
}

void print_charset( UINT cp, int w)
{
  char buf[100];
  const char *t=get_charset (cp);
  sprintf (buf,"\"%s\"", t);
  if (strlen(t))
	  print_w(buf, w);
	else
		print_w(" NULL",w);
}

int
main (int argc, char **argv)
{

  LCID lcid = GetSystemDefaultLCID ();
  UINT ansi_cp, mac_cp, oem_cp, unix_cp;
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTUNIXCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & unix_cp, sizeof (unix_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & ansi_cp, sizeof (ansi_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & oem_cp, sizeof (oem_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTMACCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & mac_cp, sizeof (mac_cp) / sizeof (WCHAR));
	char buf[100];
	char *l = getenv("LANG");
	sprintf(buf,"\"%s\"",l);
  print_w (buf, 14);
	
  sprintf (buf, "%d", lcid);
  print_w (buf, 7);
  /* Hack for UTF-8 */
  if (!strcasestr(l,"utf8"))
	  print_charset(unix_cp, 15);
  else
  	  print_w ("\"UTF8\"",15);
  print_charset(ansi_cp, 12);
  print_charset(oem_cp, 12);
  print_charset(mac_cp, 0);
  return 0;
}
