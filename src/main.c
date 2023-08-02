#include "flecs.h"
#include <stdio.h>

// Dummy system
void Sys(ecs_iter_t *it) {
  printf("system %s\n", ecs_get_name(it->world, it->system));
}

int main() {
  ecs_world_t *ecs = ecs_init();
  
  // Create three custom phases. Note that the two phases have the Phase tag,
  // which is necessary for the builtin pipeline to discover which systems it
  // should run.
  ecs_entity_t Update = ecs_new_w_id(ecs, EcsPhase);
  ecs_entity_t Physics = ecs_new_w_id(ecs, EcsPhase);
  ecs_entity_t Collisions = ecs_new_w_id(ecs, EcsPhase);

  ecs_add_pair(ecs, Physics, EcsDependsOn, Update);
  ecs_add_pair(ecs, Collisions, EcsDependsOn, Physics);

  // Create 3 dummy systems.
  ecs_system(ecs,
             {.entity = ecs_entity(ecs, {.name = "CollisionSystem",
                                         .add = {ecs_dependson(Collisions)}}),
              .callback = Sys});

  ecs_system(ecs, {.entity = ecs_entity(ecs, {.name = "PhysicsSystem",
                                              .add = {ecs_dependson(Physics)}}),
                   .callback = Sys});

  ecs_system(ecs, {.entity = ecs_entity(ecs, {.name = "GameSystem",
                                              .add = {ecs_dependson(Update)}}),
                   .callback = Sys});

  // Run pipeline
  ecs_progress(ecs, 0);

  return ecs_fini(ecs);
}