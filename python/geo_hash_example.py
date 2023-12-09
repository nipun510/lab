import overpy
import geohash

def get_geohashes(bbox, precision=7):
    api = overpy.Overpass()

    # Define the bounding box (bbox)
    south, west, north, east = bbox
    query = f"""
    way({south},{west},{north},{east});
    (._;>;);
    out body;
    """

    result = api.query(query)

    geohashes = set()

    for way in result.ways:
        for node in way.nodes:
            lat, lon = node.lat, node.lon
            gh = geohash.encode(lat, lon, precision=precision)
            geohashes.add(gh)

    return geohashes

# Example usage
bbox_example = (37.7749, -122.4194, 37.8049, -122.3894)  # San Francisco bounding box
geohashes_result = get_geohashes(bbox_example)

print("Geohashes in the specified area:", geohashes_result)
