#include <stdio.h>
#include <flecs.h>

int main(void)
{
    ecs_world_t* ecs = ecs_mini();
    printf("Hello Test!\n");
    ecs_progress(ecs);
    return ecs_fini(ecs);
}