# Song Sibling

Song sibling is a geospatial-style engine. It was originally built for lat/long geographical data, but due to my own interests in music, I repurposed it to be about songs. 
Song Sibling lets you explore ~1,000 songs and find by audio features like, finding a song's closest sibling, or seeing what songs are considered similar. 

Built as a C++ learning project: implements a k-d tree from scratch, BFS-based clustering 
(DBSCAN-inspired), a REST-style HTTP server, and an interactive JS/Canvas frontend.
<img width="1440" height="812" alt="Screenshot 2026-08-26 at 2 12 31 PM" src="https://github.com/user-attachments/assets/749bb06a-c0a9-4fdf-ab9e-d7c62f5c37f2" />


## What it does

- **Search a song**: and see it plotted by danceability (x) and energy (y)
- **"Find its sibling"**: drag a slider to expand/shrink a similarity radius around the 
  selected song; a receipt panel lists every song within that radius
- **"Find families"**: cluster the entire dataset into similarity tiers (twins/siblings/
  cousins/strangers) using BFS-based clustering, each rendered in a distinct color

## How it works

- **k-d tree** (`kdtree.h`): built by recursively splitting points on alternating x/y axes 
  at the median, giving ~O(log n) nearest-neighbor and radius queries instead of O(n) brute force
- **Clustering** (`clusterPoints`): BFS/flood-fill using radius queries to group points that 
  are transitively close to each other, similar to a simplified DBSCAN
- **HTTP server** (`cpp-httplib`): exposes `/points`, `/nearest`, `/radius`, and `/cluster` 
  endpoints returning JSON
- **Frontend**: Basic JS + HTML canvas, no frameworks; fetches from the local server and 
  renders points, click/search interactions, and live-updating visualizations

## Tech

C++17 · CMake · cpp-httplib · vanilla JavaScript · HTML Canvas · Python (data reformatting)

## Running it

**Backend:**
```bash
mkdir build && cd build
cmake ..
make
cd ..
./build/geo_nn
```
Server runs at `http://localhost:8080`.

**Frontend:**
Open `web/index.html` in a browser (with the server running).

**Data:**
Dataset is derived from the [Spotify Tracks Dataset on Kaggle](https://www.kaggle.com/datasets/maharshipandya/-spotify-tracks-dataset). 
`reformat_spotify.py` converts the raw Kaggle CSV into the `name,x,y` format the engine expects 
(danceability as x, energy as y).

## What I learned

Through this mini-project I developed skills relevant to autonomous systems like spatial data structures, nearest-neighbor search, and clustering. 

Highlights:
- Implementing a k-d tree and its pruning logic from scratch (no STL spatial structures used)
- Recursive tree construction and traversal, memory management with raw pointers
- Benchmarking against brute-force search (~1.75x speedup at this dataset size)
- Building and consuming a small REST API in C++
- Async JS (fetch/promises), Canvas rendering, and basic UI/UX design

## Possible extensions

- 3D point support (would generalize directly toward LiDAR point-cloud clustering)
- Live Spotify API integration instead of a static dataset
- WebAssembly build to run the C++ engine directly in-browser
