type timer = { start_time : float; mutable end_time : float }

val start : unit -> timer
val stop : timer -> unit
val elapsed_seconds : timer -> float
