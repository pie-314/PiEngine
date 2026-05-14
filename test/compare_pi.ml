let read_and_clean_pi filename =
  let ic = open_in filename in
  let len = in_channel_length ic in
  let buffer = Buffer.create len in
  (try
     while true do
       let line = input_line ic in
       String.iter (fun c -> if not (Char.lowercase_ascii c |> (fun c -> c = ' ' || c = '\n' || c = '\r' || c = '\t')) then Buffer.add_char buffer c) line
     done
   with End_of_file -> close_in ic);
  Buffer.contents buffer

let compare_pi real_pi calc_pi =
  let real_len = String.length real_pi in
  let calc_len = String.length calc_pi in
  let total_compared = min real_len calc_len in
  let rec find_mismatch i =
    if i >= total_compared then i
    else if real_pi.[i] <> calc_pi.[i] then i
    else find_mismatch (i + 1)
  in
  let accurate_digits = find_mismatch 0 in
  (real_len, calc_len, total_compared, accurate_digits)

let () =
  if Array.length Sys.argv <> 3 then (
    Printf.fprintf stderr "Usage: %s <real_pi.txt> <calculated_pi.txt>\n" Sys.argv.(0);
    exit 1
  );

  let real_file = Sys.argv.(1) in
  let calc_file = Sys.argv.(2) in

  Printf.printf "   Pi Value Comparison Test\n\n";
  
  Printf.printf "Loading real pi from: %s\n" real_file;
  let real_pi = read_and_clean_pi real_file in
  let real_len = String.length real_pi in
  Printf.printf "Loaded %d digits\n\n" real_len;

  Printf.printf "Loading calculated pi from: %s\n" calc_file;
  let calc_pi = read_and_clean_pi calc_file in
  let calc_len = String.length calc_pi in
  Printf.printf "Loaded %d digits\n\n" calc_len;

  let (real_len, calc_len, total_compared, accurate_digits) = compare_pi real_pi calc_pi in

  Printf.printf "   Comparison Results\n";
  Printf.printf "Real pi digits:       %d\n" real_len;
  Printf.printf "Calculated pi digits: %d\n" calc_len;
  Printf.printf "Total compared:       %d\n\n" total_compared;

  if accurate_digits = total_compared then
    Printf.printf "ALL DIGITS MATCH! (%d/%d accurate)\n" accurate_digits total_compared
  else (
    let percentage = if total_compared > 0 then (float_of_int accurate_digits *. 100.0 /. float_of_int total_compared) else 0.0 in
    Printf.printf "Accurate digits: %d/%d (%.2f%%)\n" accurate_digits total_compared percentage;
    
    if accurate_digits < total_compared && accurate_digits < 100 then (
      Printf.printf "\nFirst mismatch at position %d:\n" accurate_digits;
      Printf.printf "  Real:       %c\n" real_pi.[accurate_digits];
      Printf.printf "  Calculated: %c\n" calc_pi.[accurate_digits]
    )
  );

  if accurate_digits = total_compared then exit 0 else exit 1
