open Pi_engine_lib

let main () =
  let digits = ref 1000 in
  let output_file = ref "" in
  let verbose = ref false in

  let spec = [
    ("-d", Arg.Set_int digits, " Number of digits of pi to compute (default: 1000)");
    ("--digits", Arg.Set_int digits, " Number of digits of pi to compute (default: 1000)");
    ("-o", Arg.Set_string output_file, " Write output to file (default: stdout)");
    ("--output", Arg.Set_string output_file, " Write output to file (default: stdout)");
    ("-v", Arg.Set verbose, " Show timing and performance information");
    ("--verbose", Arg.Set verbose, " Show timing and performance information");
  ] in

  let usage_msg = "Usage: piengine [OPTIONS]\nOptions:" in
  Arg.parse spec (fun _ -> ()) usage_msg;

  let total_timer = Utils.start () in

  if !verbose then (
    Printf.eprintf "=== PiEngine Computation ===\n";
    Printf.eprintf "Target digits: %d\n" !digits;
    Printf.eprintf "Output to: %s\n\n" (if !output_file = "" then "stdout" else !output_file);
    Printf.eprintf "Starting computation...\n";
    flush stderr
  );

  let compute_timer = Utils.start () in
  let pi_str = Chudnovsky.compute_pi !digits in
  Utils.stop compute_timer;

  let oc = if !output_file = "" then stdout else open_out !output_file in
  Printf.fprintf oc "%s\n" pi_str;
  flush oc;
  if !output_file <> "" then close_out oc;

  Utils.stop total_timer;

  if !verbose then (
    Printf.eprintf "\n=== Timing Results ===\n";
    Printf.eprintf "Computation time: %.4f seconds\n" (Utils.elapsed_seconds compute_timer);
    Printf.eprintf "Total time: %.4f seconds\n" (Utils.elapsed_seconds total_timer);
    if !output_file <> "" then Printf.eprintf "Output written to: %s\n" !output_file;
    flush stderr
  )

let () = main ()
