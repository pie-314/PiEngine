type timer = { start_time : float; mutable end_time : float }

let start () = { start_time = Unix.gettimeofday (); end_time = 0.0 }

let stop t = t.end_time <- Unix.gettimeofday ()

let elapsed_seconds t = t.end_time -. t.start_time
