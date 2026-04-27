#GROUP 6
#Parsley Njoroge - Sct211-0007/2023
#Patrick Leon - Sct211-0003/2023

from PIL import Image, ImageDraw, ImageFont

def create_centered_banner():
    #  Setup the canvas
    width, height = 1000, 400
    brown_bg = (139, 69, 19)
    image = Image.new("RGB", (width, height), brown_bg)
    draw = ImageDraw.Draw(image)

    #  Load the font
    try:
        font = ImageFont.truetype("arial.ttf", 80)
    except IOError:
        font = ImageFont.load_default()

    #  Define the text and colors
    text_part1 = "JKUAT "
    text_part2 = "ROCKS"
    full_text = text_part1 + text_part2  
    
    green = (0, 255, 0)
    red = (255, 0, 0)

    #  Calculate total dimensions of the combined text
    total_width = draw.textlength(full_text, font=font)
    
    # Get the bounding box to find the true height of the text
    left, top, right, bottom = draw.textbbox((0, 0), full_text, font=font)
    total_height = bottom - top

    #  Calculate perfectly centered X and Y coordinates
    start_x = (width - total_width) / 2
    start_y = (height - total_height) / 2

    #  Draw Part 1 (Green) at the calculated center-start position
    draw.text((start_x, start_y), text_part1, font=font, fill=green)

    #  Get the width of Part 1 
    part1_width = draw.textlength(text_part1, font=font)

    #  Draw Part 2 (Red) immediately after Part 1
    draw.text((start_x + part1_width, start_y), text_part2, font=font, fill=red)

    # Display and save
    image.show()
    image.save("jkuat_centered_banner.png")
    print("Centered banner saved successfully!")

if __name__ == "__main__":
    create_centered_banner()
