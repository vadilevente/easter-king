# Easter Sprinkling King — OS Coursework (ELTE)

This was a university assignment for the **Operating Systems** course at ELTE (Eötvös Loránd University, Budapest). The task involved classic IPC concepts: **fork**, **pipes**, and **signals** between parent and child processes.

---

## The Task

A *Head Rabbit* organizes an Easter sprinkling competition among boy rabbits. The one who collects the most red eggs from girl rabbits becomes the **Easter Sprinkling King**.

**Data stored per rabbit:**
- Name (e.g. *Tapsi Füles*)
- Sprinkling poem (e.g. *"Roses are red, violets are blue, I came to sprinkle, good luck to you!"*)
- Number of eggs collected (0 at registration)

---

## Program Flow

1. **Registration** — rabbits sign up with a name and poem; eggs start at 0; data is persisted to a file.
2. **Easter Monday** — the Head Rabbit (parent process) forks a **child process per rabbit**.
3. Each child:
   - Sends a **signal** (`SIGUSR1`) to the parent upon arrival at the girl rabbits' house
   - Recites its poem (printed to screen)
   - Receives **1–20 eggs** randomly, based on how much the poem was liked
   - Prints the egg count to screen
   - Sends the egg count back to the parent via a **pipe**
4. The parent:
   - Records the egg counts received from each pipe
   - Announces the rabbit with the most eggs as **Easter Sprinkling King** (name + egg count printed to screen)

---

## Concepts Demonstrated

| Concept | Usage |
|---|---|
| `fork()` | One child process per registered rabbit |
| `pipe()` | Child → parent communication for egg counts |
| `SIGUSR1` signal | Child notifies parent upon "arrival" |
| File I/O | Persistent storage of rabbit data |
| Dynamic memory | `malloc` / `realloc` / `free` for the rabbit list |

---

## Building & Running

```bash
gcc -o easterking easterking.c sprinkle.c && ./easterking
```

### Commands

| Command | Description |
|---|---|
| `list` | Print all registered rabbits |
| `new` | Register a new rabbit |
| `edit` | Modify a rabbit's name, poem, or egg count |
| `delete` | Remove a rabbit from the list |
| `sprinkle` | Send all rabbits out to collect eggs (forks children) |
| `king` | Announce the rabbit with the most eggs as King |
| `help` | Show available commands |
| `quit` | Save data to file and exit |

---

## File Format (`rabbits.dat`)

Three lines per rabbit, no header:

```
Pisti
Roses are red, violets are blue, I came to sprinkle, good luck to you!
77
Zoli
I walked through fields and meadows wide, to sprinkle you this Eastertide!
508
Sanyi
Don't be angry, don't be mad, this little sprinkle makes hearts glad!
117
```
