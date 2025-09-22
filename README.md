# Tetris 

A simple and hackable (modern) Tetris base written in C (with raylib).


https://github.com/user-attachments/assets/9abfa456-a9a4-4945-a492-bbca095223f1


Includes all modern Tetris standards, such as:
- hard drops and soft drops
- ghost block
- guideline SRS (All spins are possible)
- 90, 180, and 270 rotation 
- Delay auto shift, Auto repeat rate, and soft drop speed
- hold
- next piece queue
- lock delay

Bugs:
- lock delay has no limit

## Configuration
Under `main.c`, you will find the option to configure DAS, ARR, SFD, and key binds. The default DAS and ARR is taken from jstris.
