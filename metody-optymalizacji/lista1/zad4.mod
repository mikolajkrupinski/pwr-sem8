# Mikołaj Krupiński, zadanie 4

set classes; # Nazwy ćwiczeń obowiązkowych
set groups := {1..4}; # Numery grup
set days_of_week := {1..5}; # Numery dni tygodnia, 1 - poniedziałek, ..., 5 - piątek
param max_hours_per_day, >= 0; # Maksymalna liczba godzin ćwiczeń w ciągu dnia
param groups_days{classes, groups}, integer, >= 1, <= 5; # Numery dni tygodnia, w których odbywają się ćwiczenia
param groups_start_time{classes, groups}, >= 0, <= 24; # Czas rozpoczęcia ćwiczeń
param groups_end_time{classes, groups}, >= 0, <= 24; # Czas zakończenia ćwiczeń
param groups_preference{classes, groups}, integer, >= 0, <= 10; # Punkty preferencji grupy ćwiczeniowej
set sport_groups := {1..3}; # Numery grup sportowych
param sport_groups_days{sport_groups}, integer, >= 1 , <= 5; # Numery dni tygodnia, w których odbywają się zajęcia sportowe
param sport_groups_start_time{sport_groups}, >= 0, <= 24;  # Czas rozpoczęcia zajęć sportowych
param sport_groups_end_time{sport_groups}, >= 0, <= 24; # Czas zakończenia zajęć sportowych
param lunch_break_start, >= 0, <= 24; # Najwcześniejsza godzina początku przerwy na lunch
param lunch_break_end, >= 0, <= 24; # Najpóźniejsza godzina końca przerwy na lunch
param min_lunch_break_time, >= 0; # Minimalny czas przerwy na lunch

# 4.2 parameters
param min_preference; # Minimalna liczba punktów preferencji grupy ćwiczeniowej
set allowed_days_of_week within days_of_week; # Dni tygodnia, w których mogą odbywać się ćwiczenia

var x{classes, groups}, binary; # x[c, g] = 1, jeśli student wybiera grupę g ćwiczeń c
var sport_group_selection{sport_groups}, binary; # sport_group_selection[g] = 1, jeśli student wybiera grupę g zajęć sportowych

maximize groups_preference_sum: # Suma punktów preferencji wybranych grup
    sum{c in classes, g in groups} (x[c, g] * groups_preference[c, g]);

s.t. one_group_per_class {c in classes}: # Student zapisy do dokładnie jednej grupy z każdego przedmiotu
    sum{g in groups} x[c, g] = 1;

s.t. at_least_one_sport: # Student ma okno na co najmniej jedne zajęcia sportowe
    sum{g in sport_groups} sport_group_selection[g] >= 1;

s.t. no_overlapping_groups {c1 in classes, g1 in groups, c2 in classes, g2 in groups: # Zajęcia nie mogą się pokrywać
    c1 != c2 and groups_days[c1, g1] == groups_days[c2, g2] and
    groups_start_time[c1, g1] <= groups_start_time[c2, g2] and groups_start_time[c2, g2] <= groups_end_time[c1, g1]}:
        x[c1, g1] + x[c2, g2] <= 1;
        

s.t. no_overlapping_groups_with_sport {c1 in classes, g1 in groups, sg in sport_groups: # Zajęcia sportowe nie mogą się pokrywać z zajęciami
    groups_days[c1, g1] == sport_groups_days[sg] and
    ((groups_start_time[c1, g1] <= sport_groups_start_time[sg] and sport_groups_start_time[sg] <= groups_end_time[c1, g1]) or
    (sport_groups_start_time[sg] <= groups_start_time[c1, g1] and groups_start_time[c1, g1] <= sport_groups_end_time[sg]))}:
        x[c1, g1] + sport_group_selection[sg] <= 1;

s.t. max_hours_per_day_satisfaction {d in days_of_week}: # Maksymalna liczba godzin ćwiczeń w ciągu dnia
    sum{c in classes, g in groups: groups_days[c, g] = d} (x[c, g] * (groups_end_time[c, g] - groups_start_time[c, g])) <= max_hours_per_day;

s.t. lunch_break_satisfaction {d in days_of_week}: # Minimalny czas przerwy na lunch
    (sum{c in classes, g in groups: groups_days[c, g] == d}
        (x[c, g] * (min(lunch_break_end, groups_end_time[c, g]) - max(lunch_break_start, groups_start_time[c, g]))))
    +
    (sum{sg in sport_groups: sport_groups_days[sg] == d}
        (sport_group_selection[sg] * (min(lunch_break_end, sport_groups_end_time[sg]) - max(lunch_break_start, sport_groups_start_time[sg]))))
    <= (lunch_break_end - lunch_break_start) - min_lunch_break_time;

# 4.2 constraints
s.t. min_preference_satisfaction {c in classes, g in groups}: # Minimalna liczba punktów preferencji grupy ćwiczeniowej
    groups_preference[c, g] >= x[c, g] * min_preference;

s.t. forbidden_days_satisfaction {c in classes, g in groups}: # Zajęcia nie mogą odbywać się w dni, które są zabronione
    sum{d in days_of_week: groups_days[c, g] == d and d not in allowed_days_of_week} x[c,g] = 0;

solve;

printf "=========================================\n";

printf "Sum of preference points = %d\n", groups_preference_sum.val;

for {c in classes, g in groups : x[c, g] == 1} {
    printf "%s : group %d (%d, %.1f-%.1f)\n", c, g, groups_days[c, g], groups_start_time[c, g], groups_end_time[c, g];
}

for {sg in sport_groups : sport_group_selection[sg] == 1} {
    printf "Sport : group %d (%d, %.1f-%.1f)\n", sg, sport_groups_days[sg], sport_groups_start_time[sg], sport_groups_end_time[sg];
}

printf "=========================================\n";

end;
