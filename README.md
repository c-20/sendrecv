# sendrecv
send and receive coordinate data

send program will create an applet to upload one packet per five geolocation samples. It will also open recv.

recv program will create a page that renders colour (ANSI) text of the data from send organised into folders (jia) and days (ri).

recv also draws a simple map.

Both send and recv should run in /cgi-bin/ or equivalent.

Requires HTTPS in many browsers unless coordinates are externally sourced

Next step: externally sourced coordinates + mouse hijack

Todo: Update send to update in real-time + have recv check server for changes

Todo: map zoom and path highlight
