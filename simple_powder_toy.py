#   Controls:
#     - Click and hold to drop powder
#     - Use mouse wheel to increase dropping radius and amount

import numpy as np
import pygame

pygame.init()
SCALE = 6
CANVAS_SIZE = (108*SCALE, 192*SCALE)
FPS = 60
DROP_SPEED = 4

POWDER_DENSITY = 0.5
circle_size = 5
powder_amount = round(circle_size * POWDER_DENSITY)**2

class Powder():
    def __init__(self):
        self.color = 255
        self.y, self.x = [i+np.random.randint(-circle_size, circle_size+1) for i in cursor_pos[::-1]]
                                  
        if self.x >= CANVAS_SIZE[1] - 1:
            self.x = CANVAS_SIZE[1] - 1
        if self.x <= 0:
            self.x = 0
        if self.y >= CANVAS_SIZE[0]:
            self.y = CANVAS_SIZE[0] - 1
        if self.y <= 0:
            self.y = 0
    
    def can_drop(self):
        try:
            return canvas[self.y + self.move_y, self.x + self.move_x] == 0
        except(IndexError):
            return False
    
    def find_drop_values(self):
        while not self.can_drop():
                if self.move_x < 0:
                    self.move_x += 1
                    
                if self.move_x > 0:
                    self.move_x -= 1

                if not self.can_drop():
                    if self.move_y > 0:
                        self.move_y -= 1

                if self.move_x == 0 and self.move_y == 0:
                    break

    def fall(self):
        self.move_x = round(np.random.normal(0, 1))
        self.move_y = DROP_SPEED
        self.find_drop_values()
        
        canvas[self.y, self.x] = 0
        self.x += self.move_x
        self.y += self.move_y
        canvas[powder.y, powder.x] = powder.color

        

canvas = np.zeros(CANVAS_SIZE)
screen = pygame.display.set_mode(CANVAS_SIZE[::-1])
clock = pygame.time.Clock()
powder_list = []

done = False
drop_powder = False
cursor_pos = pygame.mouse.get_pos()

while not done:
    if drop_powder:
        cursor_pos = pygame.mouse.get_pos()
        try:
            for _ in range(powder_amount):
                powder_list.append(Powder())
        except(IndexError):
            pass
        
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
            pygame.quit()
            
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                drop_powder = True
                
            if event.button == 4:
                circle_size += 1
                powder_amount = round(circle_size * POWDER_DENSITY)
                
            if event.button == 5:
                if circle_size > 0:
                    circle_size -= 1
                powder_amount = round(circle_size * POWDER_DENSITY)
                
        if event.type == pygame.MOUSEBUTTONUP and event.button == 1:
            drop_powder = False
    if done:
        break
    for powder in powder_list:
        powder.fall()
    
    screen.blit(pygame.surfarray.make_surface(canvas.T), (0,0))
    pygame.display.update()
    clock.tick(FPS)
