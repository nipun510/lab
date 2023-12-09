import folium
import geohash
import webbrowser

def map_call():
  #lat, long = geohash.decode('qqu57') # get the center of the geohash
  # create a map canvas
  lat, long = 28.5720, 77.3694
  gh = geohash.encode(lat, long, 6)
  m = folium.Map(
      location=[lat,long], # set the center location of the map
      zoom_start=9.5
  )
  decoded = geohash.bbox(gh) # decode the geohash
  W = decoded["w"]
  E = decoded["e"]
  N = decoded["n"]
  S = decoded["s"]
  # create each point of the rectangle
  upper_left = (N, W)
  upper_right = (N, E)
  lower_right = (S, E)
  lower_left = (S, W)
  edges = [upper_left, lower_right]
  # create rectangle object and add it to the map canvas
  folium.Rectangle(
      bounds=edges,
      color="blue",
      fill_color="green",
      weight=1,
      popup='qqu57',
  ).add_to(m)
  map_html = 'noide_map.html'
  m.save(map_html)
  webbrowser.open(map_html)

map_call()
