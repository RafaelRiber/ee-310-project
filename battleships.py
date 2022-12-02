import sys

DIM = (10,10)
WATER = 0
SHIP = 1
HIT = 2
MISS = 3
player_turn = 0
player_1_board = [
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,1,0,0,0,0,0,0,0],
    [0,0,1,0,0,0,0,0,0,0],
    [0,0,1,0,0,0,0,1,0,0],
    [0,0,0,0,0,0,0,1,0,0],
    [0,0,0,0,0,0,0,1,0,0],
    [0,0,1,1,1,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0]
]
player_2_board = [
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,1,1,1,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,1,1,1,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,1,0,0,0,0,0,0,0],
    [0,0,1,0,0,0,0,0,0,0],
    [0,0,1,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0]
]

player_1_op_board = [
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
]

player_2_op_board = [
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0],
]

player_1_ships = [
    [[(2,1),True], [(2,2), True], [(2,3),True]],
    [[(7,3),True], [(7,4), True], [(7,5),True]],
    [[(2,6),True], [(3,6), True], [(4,6),True]],
]
player_2_ships = [
    [[(2,1),True], [(3,1), True], [(4,1),True]],
    [[(6,3),True], [(7,3), True], [(8,3),True]],
    [[(2,6),True], [(2,7), True], [(2,8),True]],
]

game_true = True
player_boards = [player_1_board, player_2_board]
opponent_boards = [player_1_op_board, player_2_op_board]
player_ships = [player_1_ships, player_2_ships]
def print_gameboards():
    global player_turn
    players_board = player_boards[player_turn]
    opponents_board = opponent_boards[player_turn]
    print("your gameboard:")
    for row in players_board:
        s = ""
        for elem in row:
            if elem == WATER:
                s += '🟦'
            elif elem == SHIP:
                s += "⬛"
            elif elem == HIT:
                s += "🟥"
            elif elem == MISS:
                s += "🟪"

        print(s)
    
    print("their gameboard:")
    for row in opponents_board:
        s = ""
        for elem in row:
            if elem == WATER:
                s += '🟦'
            elif elem == MISS:
                s += "🟪"
            elif elem == HIT:
                s += "🟥"
        print(s)

def update_game_boards(target_player_id, coord, hit):
    global player_turn
    if hit:
        player_boards[target_player_id][coord[1]][coord[0]] = HIT
        opponent_boards[player_turn][coord[1]][coord[0]] = HIT
    else:
        opponent_boards[player_turn][coord[1]][coord[0]] = MISS
        player_boards[target_player_id][coord[1]][coord[0]] = MISS
    
def game_logic(coord):
    global player_turn
    enemey_ships = player_ships[(player_turn + 1) % 2]
    hit = False
    ships_alive = 0
    for ship in enemey_ships:
        for i in range(len(ship)):
            ship_blk = ship[i]
            ship_coord = ship_blk[0]

            #set to not alive
            if ship_coord == coord:
                ship[i][1] = False
                print(" YOU HIT THE ENEMEY SHIP!")
                hit = True
            if ship[i][1]:
                ships_alive += 1
    
    if not hit:
        print("sorry. You did not hit the enemy ship.")
    update_game_boards(target_player_id=(player_turn + 1) % 2, coord=coord, hit=hit)
    if ships_alive == 0:
        game_true = False
            

def game_loop():
    global player_turn
    print(f"player {player_turn + 1}'s turn.")
    input("press enter when the other player cannot see the screen")
    print_gameboards()
    
    coord_s = input("enter coordinate 'A B' to fire: ").split(" ")
    coord = None
    try:
        print("invalid coordinate")
        coord = (int(coord_s[0]), int(coord_s[1]))
        print(f"firing at {coord}!")
        game_logic(coord)
    except:
        print("invalid coordinate. Skipping turn!")

    
    player_turn = (player_turn + 1) % 2



def main():
    while(game_true):
        game_loop()
    winner = (player_turn + 1) % 2
    print(f"player {winner + 1} wins!")



if __name__ == "__main__":
    main()