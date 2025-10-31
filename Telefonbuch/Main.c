// Bibliotheken einbetten
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
// Pfad angeben
char Filepath[] = "C:\\Users\\U66384\\Repositories\\Telefonbuch\\Telefonbuch\\Daten.csv";
// Variabeln für Kontake erstellen und Werte zuweisen
struct ContactData
{
   char FirstName[51];
   char lastName[101];
   char Phonenumber[51];
};

struct ContactData read;
struct ContactData write;

char SearchObject[101];

// Funktion um das Usermenu aufzurufen
void Usermenu()
{
   printf("===== TELEFONBUCH =====\n");
   printf("1. Alle Dateien anzeigen\n");
   printf("2. Neue Datei erfassen\n");
   printf("3. Nach Datei suchen\n");
   printf("4. Datei bearbeiten\n");
   printf("5. Datei entfernen\n");
   printf("0. Beenden\n");
   printf("========================\n");
}
// Überprüfung der Schweizer Nummer
bool istSchweizerNummer(const char *nummer)
{
   return strncmp(nummer, "+41", 3) == 0 ||
          strncmp(nummer, "0041", 4) == 0 ||
          strncmp(nummer, "079", 3) == 0 ||
          strncmp(nummer, "076", 3) == 0 ||
          strncmp(nummer, "044", 3) == 0;
}
// Hauptfunktion
int main()
{
   int Options;

   do
   {

      // Auswahl speichern
      Usermenu();
      printf("Ihre Auswahl: ");
      scanf("%d", &Options);
      // Ja nach Auswahl zutreffenden case ausführen
      switch (Options)
      {
      case 1: // Alle Dateien anzeigen
      {
         // Datei lesen/ öffnen
         FILE *Data = fopen(Filepath, "r");
         if (Data == NULL) // Wenn Datei leer ist, dann..
         {
            printf("Datei konnte nicht geöffnet werden.\n");
            break;
         }

         while (fscanf(Data, "%50[^,],%100[^,],%50[^\n]\n",
                       read.FirstName, read.lastName, read.Phonenumber) == 3)
         {
            printf("\n---Kontakt---\n");
            printf("Vorname: %s\n", read.FirstName);
            printf("Nachname: %s\n", read.lastName);
            printf("Telefonnummer: %s\n", read.Phonenumber);
         }

         fclose(Data);
         break;
      }

      case 2: // Neue Datei erfassen
      {
         // leseb/schreiben /öffnen
         FILE *Data = fopen(Filepath, "a+");
         if (Data == NULL)
         {
            printf("Datei konnte nicht geoeffnet werden.\n");
            break;
         }

         printf("\n---Kontakt erfassen---\n");

         printf("Vorname: ");
         scanf("%50s", write.FirstName);

         printf("Nachname: ");
         scanf("%100s", write.lastName);

         while (getchar() != '\n')
            ;

         printf("Telefonnummer: ");
         fgets(write.Phonenumber, sizeof(write.Phonenumber), stdin);

         // Gröse von der Nummer
         size_t len = strlen(write.Phonenumber);
         if (len > 0 && write.Phonenumber[len - 1] == '\n')
         {
            write.Phonenumber[len - 1] = '\0';
         }

         if (!istSchweizerNummer(write.Phonenumber)) //  Überprüfung ob die Nummer eine Schweizer Vorwahl hat
         {
            printf("Ungueltige Telefonnummer - keine Schweizer Vorwahl erkannt.\n");
            fclose(Data);
            break;
         }

         int vorhanden = 0; // Überprüfen ob Telefonnummer schon vorhanden ist
         FILE *Check = fopen(Filepath, "r");
         if (Check != NULL)
         {
            while (fscanf(Check, "%51[^,],%101[^,],%51[^\n]\n",
                          read.FirstName, read.lastName, read.Phonenumber) == 3)
            {
               if (strcmp(read.Phonenumber, write.Phonenumber) == 0)
               {
                  vorhanden = 1;
                  break;
               }
            }
            fclose(Check);
         }

         if (vorhanden)
         {
            printf("Eintrag bereits vorhanden.\n");
            fclose(Data);
            break;
         }

         fprintf(Data, "%s,%s,%s\n", write.FirstName, write.lastName, write.Phonenumber);
         fclose(Data);
         printf("Kontakt erfolgreich gespeichert.\n");
         break;
      }

      case 3: // Nach Datei suchen
      {
         int found = 0;
         FILE *Data = fopen(Filepath, "r+");

         printf("Geben einen Suchbegriff ein:");
         scanf("%101s", SearchObject);

         while (fscanf(Data, "%50[^,],%100[^,],%50[^\n]\n",
                       read.FirstName, read.lastName, read.Phonenumber) == 3)

            if (strstr(read.FirstName, SearchObject) || strstr(read.lastName, SearchObject) ||
                strstr(read.Phonenumber, SearchObject))
            {
               printf("\n---Kontakt gefunden---\n");
               printf("Vorname: %s\n", read.FirstName);
               printf("Nachname: %s\n", read.lastName);
               printf("Telefonnummer: %s\n", read.Phonenumber);

               found = 1;
            }
         if (!found)
         {
            printf("Kein Kontakt gefunden.\n");
         }

         if (Data == NULL)
         {
            printf("Datei konnte nicht geöffnet werden.\n");
            break;
         }
         fclose(Data);
         break;
      }

      case 4: // Datei bearbeiten
      {
         // Variablen deklarieren
         char newFirstName[51];
         char newLastName[51];
         char newPhoneNumber[51];
         int found = 0;

         // Temporäre Datei öffnen
         FILE *Data = fopen(Filepath, "r");
         FILE *Temp = fopen("temp.csv", "w");
         // Falls keine Daten -> Datei nicht öffnen
         if (Data == NULL || Temp == NULL)
         {
            printf("Datei konnte nicht geöffnet werden.\n");
            break;
         }

         // Nach Suchbegriff fragen
         printf("Geben einen Suchbegriff ein:");
         scanf("%101s", SearchObject);

         while (fscanf(Data, "%51[^,],%101[^,],%51[^\n]\n", read.FirstName, read.lastName, read.Phonenumber) == 3)
         {
            if (strstr(read.FirstName, SearchObject) || strstr(read.lastName, SearchObject) ||
                strstr(read.Phonenumber, SearchObject))
            {
               printf("\n---Kontakt gefunden---\n");
               printf("Vorname: %s\n", read.FirstName);
               printf("Nachname: %s\n", read.lastName);
               printf("Telefonnummer: %s\n", read.Phonenumber);
               found = 1;
               Sleep(500); // wartet 500ms
               printf("Neuer Vorname:\n");
               scanf("%s", newFirstName);
               printf("Neuer Nachname:\n");
               scanf("%s", newLastName);

               while (getchar() != '\n')
                  ;

               printf("Neue Telefonnummer:\n");
               fgets(newPhoneNumber, sizeof(newPhoneNumber), stdin);

               size_t len = strlen(newPhoneNumber);
               if (len > 0 && newPhoneNumber[len - 1] == '\n')
               {
                  newPhoneNumber[len - 1] = '\0';
               }

               // Überprüfen, ob die neue Nummer die richtige Vorwahl hat
               if (!istSchweizerNummer(newPhoneNumber))
               {
                  printf("Falsche Vorwahl\n");
                  // Originaldaten übernehmen
                  fprintf(Temp, "%s,%s,%s\n", read.FirstName, read.lastName, read.Phonenumber);
                  continue;
               }

               //  Daten in Temp schreiben
               fprintf(Temp, "%s,%s,%s\n", newFirstName, newLastName, newPhoneNumber);
            }
            else
            {
               // Unveränderte Daten übernehmen
               fprintf(Temp, "%s,%s,%s\n", read.FirstName, read.lastName, read.Phonenumber);
            }
         }

         if (!found)
         {
            printf("Kein Kontakt gefunden.\n");
         }

         fclose(Data);
         fclose(Temp);

         // Datei ersetzen, wenn bearbeitet
         if (found)
         {
            remove(Filepath);
            rename("temp.csv", Filepath);
            printf("Kontakt erfolgreich bearbeitet.\n");
         }
         else
         {
            remove("temp.csv"); // Falls keine Änderungen, Vorlage schliessen (Temp Datei)
         }

         break;
      }

      case 5: // Kontakt löschen
      {
         int found = 0;

         // Datei zum Lesen öffnen
         FILE *Data = fopen(Filepath, "r");
         if (Data == NULL)
         {
            printf("Datei konnte nicht geoeffnet werden.\n");
            break;
         }

         // Temporäre Datei zum Schreiben öffnen
         FILE *Temp = fopen("temp.csv", "w");

         if (Temp == NULL)
         {
            fclose(Data);
            printf("Temporäre Datei konnte nicht erstellt werden.\n");
            break;
         }

         // Suchbegriff vom Benutzer
         char SearchObject[102];
         printf("Geben Sie einen Suchbegriff ein: \n");
         printf("Bitte achten sie darauf, dass sie die Daten korrekt eingeben\n");
         scanf("%s", SearchObject);

         // Datei zeilenweise lesen
         while (fscanf(Data, "%50[^,],%100[^,],%50[^\n]\n",
                       read.FirstName, read.lastName, read.Phonenumber) == 3)
         {
            // Wenn Suchbegriff in einem Feld vorkommt → NICHT schreiben (löschen)
            if (strstr(read.FirstName, SearchObject) ||
                strstr(read.lastName, SearchObject) ||
                strstr(read.Phonenumber, SearchObject))
            {
               found = 1; // Kontakt gefunden und gelöscht
               continue;  // Nicht in Temp schreiben, überspringen
            }

            // Kontakt behalten → in Temp schreiben
            fprintf(Temp, "%s,%s,%s\n", read.FirstName, read.lastName, read.Phonenumber);
         }

         // Dateien schließen
         fclose(Data);
         fclose(Temp);

         // Wenn Kontakt gefunden → Originaldatei ersetzen
         if (found)
         {
            remove(Filepath);             // Alte Datei löschen
            rename("temp.csv", Filepath); // Neue Datei übernehmen
            printf("Kontakt erfolgreich geloescht.\n");
         }
         else
         {
            remove("temp.txt"); // Temp löschen, da keine Änderung
            printf("Kein Kontakt gefunden.\n");
         }

         break;
      }

      case 0: // Beenden
         printf("Programm beendet.\n");
         break;

      default: // Ungültige Eingabe
         printf("Ungueltige Auswahl.\n");
      }

   } while (Options != 0); // Bei Option "0" Beendet man das Programm

   return 0;
}
