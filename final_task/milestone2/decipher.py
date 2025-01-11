def process_lines(lines):
    """
    Processes a list of strings where each string contains digits.
    Alternates between creating '*' and ' ' based on the digits and outputs the result.
    
    :param lines: List of strings containing digits
    :return: List of processed strings
    """
    results = []
    for raw in lines:
        result = []
        space_or_star = 0  # 0 for '*', 1 for ' '
        for ch in raw:
            count = int(ch)  # Convert character to integer
            if space_or_star == 0:
                result.append('*' * count)  # Add count number of '*'
                space_or_star = 1  # Switch to ' '
            else:
                result.append(' ' * count)  # Add count number of ' '
                space_or_star = 0  # Switch to '*'
        results.append("".join(result))
    return results

if __name__ == "__main__":
# Example usage:
    lines = ['18141312131254144313133', '1711131213131415111313121312131', '16131211141314141312131213121', '16131225541413124313133', '1652111413141452111413161', '1613121213131414131212131312131', '521312131213141413121313343']

    output = process_lines(lines)
    for line in output:
        print(line)
