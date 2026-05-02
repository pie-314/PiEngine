let () =
  let pi = Pi_engine_lib.Chudnovsky.compute_pi 50 in
  print_endline pi
