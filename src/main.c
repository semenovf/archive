#include <libappindicator/app-indicator.h>

/* Knowledge sources:
 * http://developer.ubuntu.com/resources/technologies/application-indicators/
 * http://developer.ubuntu.com/api/ubuntu-12.04/c/appindicator/libappindicator-app-indicator.html
 * http://zetcode.com/tutorials/gtktutorial/gtkevents/
 *
 */

static gboolean time_handler(AppIndicator *indicator)
{
  time_t curtime;
  struct tm *loctime;
  static char buffer[32];

  curtime = time(NULL);
  loctime = localtime(&curtime);
  strftime(buffer, 32, "%R", loctime);
  app_indicator_set_label (indicator, buffer, NULL/*"XX:XX:XX"*/);
  return TRUE;
}

static GtkActionEntry entries[] = {
  { "FileMenu", NULL, "_File", NULL, NULL, NULL },
  { "Quit",     "application-exit", "_Quit", "<control>Q",
    "Exit the application", G_CALLBACK (gtk_main_quit) },
};
static guint n_entries = G_N_ELEMENTS (entries);

static const gchar *ui_info =
"<ui>"
"  <popup name='IndicatorPopup'>"
"    <menuitem action='Quit' />"
"  </popup>"
"</ui>";

int main (int argc, char **argv)
{
	GtkWidget *indicator_menu;
	GtkActionGroup *action_group;
	GtkUIManager *uim;
	GError *error = NULL;
	AppIndicator *indicator;

	gtk_init (&argc, &argv);

	/* Indicator */
	indicator = app_indicator_new ("simple-clock-indicator",
			"no-indicator",
			APP_INDICATOR_CATEGORY_APPLICATION_STATUS);


	/* Menus */
	action_group = gtk_action_group_new ("SimpleClockIndicatorActions");
	gtk_action_group_add_actions (action_group,
								entries, n_entries,
								indicator);
	uim = gtk_ui_manager_new ();

	g_object_set_data_full (G_OBJECT (indicator),
                          "ui-manager", uim,
                          g_object_unref);

	gtk_ui_manager_insert_action_group (uim, action_group, 0);

	if (!gtk_ui_manager_add_ui_from_string (uim, ui_info, -1, &error)) {
		g_message ("Failed to build menus: %s\n", error->message);
		g_error_free (error);
		error = NULL;
    }

	app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);
	/*app_indicator_set_attention_icon (indicator, "indicator-messages-new");*/

	app_indicator_set_title (indicator, "SimpleClockTitle");

	indicator_menu = gtk_ui_manager_get_widget (uim, "/ui/IndicatorPopup");
	app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));

	g_timeout_add(60000, (GSourceFunc) time_handler, (gpointer)indicator);
	time_handler(indicator);

	gtk_main ();

	return 0;
}
