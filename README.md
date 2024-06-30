## About
This is a bot for a chess inspired game - Rollerball : which has three different boards (7x3, 8x4, 8x2), each with its different rules and pieces. Game rules can be found in a2.pdf and its extensions to the three boards in a5.pdf.  
The assignment uses clever heuristics added into the famous minimax algorithm.   
This was made as an assignment for COL333: Aritificial Intelligence, 2023.  


## Requirements

1. gcc >= 11
2. python >= 3.7

## Quickstart

```bash
git clone https://github.com/Aniruddha-Deb/rollerball-v2 && cd rollerball-v2
make rollerball
```

If all goes well, you should have an executable called `rollerball` in `bin`. To run the GUI, launch a web server from the `web` directory.

```bash
cd web
python3 -m http.server 8080
```

You can then open [localhost:8080](http://localhost:8080) on your browser to view the GUI. Here you can select one of the three board types. 

To launch the bots (assuming you're in the directory)

```bash
./bin/rollerball -p 8181
```

You can then connect the GUI to the bots. You would also need to start another bot for black on port 8182 to join and start the game.
