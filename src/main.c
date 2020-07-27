#include <gtk/gtk.h>

#include "main_window.h"

int main(int argc, char** argv) {

  gtk_init(&argc, &argv);

  setup_main_window();

  return 0;
}