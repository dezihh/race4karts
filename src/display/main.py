import curses
import os
import time

# Beispielhafte Zeitnahme-Variablen
elapsed_time = None
start_time = None

# Datei für Ausgabe auf /dev/ttyRace
tty_race_path = '/dev/ttyRace'
tty_race = None

# Liste für vergangene Rundenzeiten
previous_times = []

# Funktion, um die Datei /dev/ttyRace zu öffnen (falls vorhanden)
def open_tty_race():
    global tty_race
    if os.path.exists(tty_race_path):
        try:
            tty_race = open(tty_race_path, 'w')
        except Exception as e:
            tty_race = None
            print(f"Fehler beim Öffnen von {tty_race_path}: {e}")

# Funktion, um Rundenzeit in /dev/ttyRace zu schreiben
def write_to_tty_race(time_str):
    if tty_race:
        try:
            tty_race.write(time_str + '\n')
            tty_race.flush()
        except Exception as e:
            print(f"Fehler beim Schreiben in {tty_race_path}: {e}")

# Lichtschranken-Callback
def lichtschranke_callback(channel=None):
    global elapsed_time, start_time
    if start_time is None:
        start_time = time.perf_counter()  # Startzeit setzen
    else:
        elapsed_time = time.perf_counter() - start_time  # Rundenzeit berechnen
        previous_times.append(elapsed_time)  # Füge die Rundenzeit zur Liste hinzu
        start_time = None  # Timer zurücksetzen

        # Rundenzeit als String
        time_str = f'Rundenzeit: {elapsed_time:.2f} s'
        # Schreibe Rundenzeit auf /dev/ttyRace, wenn es verfügbar ist
        write_to_tty_race(time_str)

# Funktion, um die Zeit im Format MM:SS:HH auszugeben
def format_time(seconds):
    minutes = int(seconds // 60)  # Ganze Minuten
    total_seconds = seconds % 60   # Restsekunden
    hundredths = int((total_seconds - int(total_seconds)) * 100)  # Berechnung der Hundertstel
    return f"{minutes:02}:{int(total_seconds):02}:{hundredths:02}"

# Funktion, um Buttons zu zeichnen
def draw_buttons(stdscr):
    stdscr.addstr(8, 2, "[ d ] Lichtschranke", curses.A_REVERSE)  # Button für 'd'
    stdscr.addstr(9, 2, "[ r ] Zurücksetzen", curses.A_REVERSE)   # Button für 'r'
    stdscr.addstr(10, 2, "[ q ] Beenden", curses.A_REVERSE)       # Button für 'q'

# Funktion, um die Benutzeroberfläche zu initialisieren
def initialize_ui(stdscr):
    stdscr.clear()
    stdscr.border(0)
    stdscr.addstr(0, 0, "Kartrennen Zeitnahme", curses.A_BOLD)
    stdscr.addstr(5, 2, 'Rundenzeit: 00:00:00')  # Initiale Anzeige
    draw_buttons(stdscr)
    stdscr.refresh()

# Funktion, um die vergangenen Zeiten anzuzeigen
def display_previous_times(stdscr):
    for idx, time_val in enumerate(previous_times):
        time_str = format_time(time_val)
        stdscr.addstr(6 + idx, curses.COLS - len(time_str) - 4, f"{idx + 1}. {time_str}")  # Um 2 Spalten nach links verschoben

# Hauptprogramm
def main(stdscr):
    global elapsed_time, start_time

    # Öffne /dev/ttyRace, falls vorhanden
    open_tty_race()

    # Mausereignisse aktivieren
    curses.mousemask(1)

    # Curses-Setup
    curses.curs_set(0)  # Cursor ausblenden
    stdscr.nodelay(1)   # Non-blocking input
    stdscr.timeout(100)  # Refresh alle 100 ms für flüssigere Updates

    # Benutzeroberfläche initialisieren
    initialize_ui(stdscr)

    # Die letzte angezeigte Zeit speichern, um unnötige Aktualisierungen zu vermeiden
    last_displayed_time = None
    last_elapsed_time = None

    while True:
        # Eingabe verarbeiten
        key = stdscr.getch()
        if key == ord('q'):
            break
        elif key == ord('r'):
            elapsed_time = None
            start_time = None
            last_displayed_time = None  # Reset für die Anzeige
            previous_times.clear()  # Leere die Liste der vergangenen Zeiten
        elif key == ord('d'):
            lichtschranke_callback()  # Simuliere den GPIO-Event mit der Taste "d"

        # Aktualisiere nur, wenn die Zeit sich geändert hat
        current_time = None
        if start_time is not None:
            current_time = time.perf_counter() - start_time

        # Zeichne nur, wenn sich die Zeit geändert hat oder beim ersten Durchlauf
        if current_time != last_displayed_time or elapsed_time != last_elapsed_time:
            stdscr.clear()
            stdscr.border(0)

            stdscr.addstr(0, 0, "Kartrennen Zeitnahme", curses.A_BOLD)

            # Zeige laufende Zeit oder 0:00 nach Reset
            if start_time is not None:
                stdscr.addstr(5, 2, f'Laufende Zeit: {format_time(current_time)}')
            elif elapsed_time is not None:
                stdscr.addstr(5, 2, f'Letzte Rundenzeit: {format_time(elapsed_time)}')
            else:
                stdscr.addstr(5, 2, 'Rundenzeit: 00:00:00')  # Anzeige nach Reset

            # Simulierte Buttons anzeigen
            draw_buttons(stdscr)

            # Zeige vergangene Rundenzeiten an
            display_previous_times(stdscr)

            stdscr.refresh()

            # Speichere die zuletzt angezeigte Zeit
            last_displayed_time = current_time
            last_elapsed_time = elapsed_time

        # Mausereignisse abfangen
        if key == curses.KEY_MOUSE:
            try:
                _, mx, my, _, mouse_event = curses.getmouse()

                # Prüfen, ob der Mausbutton gedrückt wurde und auf welchem "Button"
                if 8 <= my <= 8 and 2 <= mx <= 17:  # "d" Button Bereich
                    lichtschranke_callback()
                elif 9 <= my <= 9 and 2 <= mx <= 17:  # "r" Button Bereich
                    elapsed_time = None
                    start_time = None
                elif 10 <= my <= 10 and 2 <= mx <= 17:  # "q" Button Bereich
                    break
            except curses.error:
                # Fehlerbehandlung für curses.getmouse(), wenn keine Mausdaten vorliegen
                pass

        # Kurze Pause, um die CPU-Belastung zu reduzieren
        time.sleep(0.01)  # 10ms Pause, um die CPU-Belastung zu verringern

    # Schließe /dev/ttyRace, falls geöffnet
    if tty_race:
        tty_race.close()

# Start der curses-Anwendung
curses.wrapper(main)
