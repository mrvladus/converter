#include <strings.h>
#include <glib/gi18n.h>
#include <adwaita.h>
#include <vte/vte.h>

#include "config.h"

GtkBuilder *builder;
GObject *window;
GObject *headerbar, *spinner, *info_btn, *about_dialog;
GObject *select_btn, *select_dialog, *select_row;
GString *name, *path;
GObject *formats_row, *formats_list;
GObject *convert_btn, *cancel_btn;
GObject *terminal_window, *terminal;
GPid pid; // Process id of command that will be running in vte-terminal

void on_cancel_btn_clicked() {
	GString *cmd = g_string_new("kill -9 ");
	g_string_append_printf(cmd, "%d", pid);
	system(cmd->str); // Kill convertation process
} 

void on_convertation_end() {
	g_object_set(terminal_window, "visible", false, NULL);
	g_object_set(convert_btn, "visible", true, NULL);
	g_object_set(cancel_btn, "visible", false, NULL);
	g_object_set(spinner, "visible", false, NULL);
}

void on_convertation_start(VteTerminal *term, GPid gpid, GError *error, gpointer user_data) {
	g_object_set(terminal_window, "visible", true, NULL);
	g_object_set(convert_btn, "visible", false, NULL);
	g_object_set(cancel_btn, "visible", true, NULL);
	g_object_set(spinner, "visible", true, NULL);
	g_object_set(spinner, "spinning", true, NULL);
	pid = gpid; // Set pid so we can kill it by pressing cancel button
}

void on_convert_btn_clicked() {
	// Get selected format
	const char *format = gtk_string_list_get_string(GTK_STRING_LIST(formats_list), adw_combo_row_get_selected(ADW_COMBO_ROW(formats_row)));
	// Create output file path
	GString *out = g_string_new(path->str);
	// Add format to output filename
	g_string_append(out, format);
	// Construct command and run in vte-terminal
	if (strcmp(format, ".avi") == 0) {
		char *cmd[] = {"ffmpeg", "-i", path->str, "-c:v", "libxvid", "-c:a", "ac3", "-qscale", "3", "-y", out->str, NULL};
		vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT, NULL, cmd, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, -1, NULL, on_convertation_start, NULL);
	}
	else if (strcmp(format, ".mkv") == 0) {
		char *cmd[] = {"ffmpeg", "-i", path->str, "-c", "copy", "-y", out->str, NULL};
		vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT, NULL, cmd, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, -1, NULL, on_convertation_start, NULL);
	}
	else if (strcmp(format, ".webm") == 0) {
		char *cmd[] = {"ffmpeg", "-i", path->str, "-c:v", "libvpx", "-c:a", "libvorbis", "-crf", "10", "-b:v", "1M", "-y", out->str, NULL};
		vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT, NULL, cmd, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, -1, NULL, on_convertation_start, NULL);
	}
}

void on_select_btn_clicked() {
	gtk_widget_show(GTK_WIDGET(select_dialog));
}

void on_select_dialog_response(GtkDialog *dialog, int response) {
	gtk_widget_hide(GTK_WIDGET(dialog));
	if (response == GTK_RESPONSE_OK) {
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		g_autoptr(GFile) file = gtk_file_chooser_get_file(chooser);
		path = g_string_new(g_file_get_path(file));
		name = g_string_new(g_file_get_basename(file));
		// g_print("%s %s\n", name, path);
		g_object_set(select_row, "subtitle", name->str, NULL);
		g_object_set(convert_btn, "sensitive", true, NULL);
	}
}

void on_info_btn_clicked() {
	gtk_widget_show(GTK_WIDGET(about_dialog));
}

void connect_signals() {
	g_signal_connect(info_btn, "clicked", G_CALLBACK(on_info_btn_clicked), NULL);
	g_signal_connect(select_btn, "clicked", G_CALLBACK(on_select_btn_clicked), NULL);
	g_signal_connect(select_dialog, "response", G_CALLBACK(on_select_dialog_response), NULL);
	g_signal_connect(convert_btn, "clicked", G_CALLBACK(on_convert_btn_clicked), NULL);
	g_signal_connect(cancel_btn, "clicked", G_CALLBACK(on_cancel_btn_clicked), NULL);
	g_signal_connect(terminal, "child-exited", G_CALLBACK(on_convertation_end), NULL);
}

void get_objects() {
	window = gtk_builder_get_object(builder, "window");
	headerbar = gtk_builder_get_object(builder, "headerbar");
	spinner = gtk_builder_get_object(builder, "spinner");
	info_btn = gtk_builder_get_object(builder, "info_btn");
	about_dialog = gtk_builder_get_object(builder, "about_dialog");
	select_row = gtk_builder_get_object(builder, "select_row");
	select_btn = gtk_builder_get_object(builder, "select_btn");
	select_dialog = gtk_builder_get_object(builder, "select_dialog");
	formats_row = gtk_builder_get_object(builder, "formats_row");
	formats_list = gtk_builder_get_object(builder, "formats_list");
	convert_btn = gtk_builder_get_object(builder, "convert_btn");
	cancel_btn = gtk_builder_get_object(builder, "cancel_btn");
	terminal_window = gtk_builder_get_object(builder, "terminal_window");
	terminal = gtk_builder_get_object(builder, "terminal");
}

void activate(GtkApplication *app) {
	// Add custom styles
	GtkCssProvider *css_provider = gtk_css_provider_new();
	gtk_css_provider_load_from_resource(css_provider, "/org/mrvladus/converter/styles.css");
	gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(css_provider), 600);
	// Create builder, get objects and connect signals
	builder = gtk_builder_new_from_resource("/org/mrvladus/converter/window.ui");
	get_objects();
	connect_signals();
	// Show window
	g_object_set(window, "application", app, NULL);
	gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
	// Load locales
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	textdomain(GETTEXT_PACKAGE);
	// Create and run application
	AdwApplication *app = adw_application_new("org.mrvladus.converter", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), GTK_APPLICATION(app));
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
