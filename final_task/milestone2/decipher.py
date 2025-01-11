lines = [
    "18141312131254144313133",
    "1711131213131415111313121312131",
    "16131211141314141312131213121",
    "16131225541413124313133",
    "1652111413141452111413161",
    "1613121213131414131212131312131",
    "521312131213141413121313343"
]

for raw in lines:
    result = []
    space_or_star = 0  
    for ch in raw:
        count = int(ch)  
        if space_or_star == 0:
            
            result.append('*' * count)
            space_or_star = 1
        else:

            result.append(' ' * count)
            space_or_star = 0
   
    print("".join(result))
