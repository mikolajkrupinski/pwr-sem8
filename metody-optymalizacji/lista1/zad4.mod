set classes;
set groups := {1..4};
set days_of_week := {1..5};
param max_hours_per_day, >= 0;
param groups_days{classes, groups}, integer, >= 1, <= 5;
param groups_start_time{classes, groups}, >= 0, <= 24;
param groups_end_time{classes, groups}, >= 0, <= 24;
param groups_preference{classes, groups}, integer, >= 0, <= 10;
set sport_groups := {1..3};
param sport_groups_days{sport_groups}, integer, >= 1 , <= 5;
param sport_groups_start_time{sport_groups}, >= 0, <= 24;
param sport_groups_end_time{sport_groups}, >= 0, <= 24;
param lunch_break_start, >= 0, <= 24;
param lunch_break_end, >= 0, <= 24;
param min_lunch_break_time, >= 0;

# 4.2 parameters
param min_preference;
set allowed_days_of_week within days_of_week;

var x{classes, groups}, binary;
var sport_group_selection{sport_groups}, binary;

maximize groups_preference_sum:
    sum{c in classes, g in groups} (x[c, g] * groups_preference[c, g]);

s.t. one_group_per_class {c in classes}:
    sum{g in groups} x[c, g] = 1;

s.t. at_least_one_sport:
    sum{g in sport_groups} sport_group_selection[g] >= 1;

s.t. no_overlapping_groups {c1 in classes, g1 in groups, c2 in classes, g2 in groups:
    c1 != c2 and groups_days[c1, g1] == groups_days[c2, g2] and
    groups_start_time[c1, g1] <= groups_start_time[c2, g2] and groups_start_time[c2, g2] <= groups_end_time[c1, g1]}:
        x[c1, g1] + x[c2, g2] <= 1;
        

s.t. no_overlapping_groups_with_sport {c1 in classes, g1 in groups, sg in sport_groups:
    groups_days[c1, g1] == sport_groups_days[sg] and
    ((groups_start_time[c1, g1] <= sport_groups_start_time[sg] and sport_groups_end_time[sg] <= groups_start_time[c1, g1]) or
    (sport_groups_start_time[sg] <= groups_start_time[c1, g1] and groups_start_time[c1, g1] <= sport_groups_end_time[sg]))}:
        x[c1, g1] + sport_group_selection[sg] <= 1;

s.t. max_hours_per_day_satisfaction {d in days_of_week}:
    sum{c in classes, g in groups: groups_days[c, g] = d} (x[c, g] * (groups_end_time[c, g] - groups_start_time[c, g])) <= max_hours_per_day;

s.t. lunch_break_satisfaction {d in days_of_week}:
    (sum{c in classes, g in groups: groups_days[c, g] == d and
        lunch_break_start <= groups_start_time[c, g] and groups_start_time[c, g] < lunch_break_end}
        (x[c, g] * (min(lunch_break_end, groups_end_time[c, g]) - groups_start_time[c, g])))
    +
    (sum{c in classes, g in groups: groups_days[c, g] == d
        and lunch_break_start < groups_end_time[c, g] and groups_end_time[c, g] <= lunch_break_end
        and groups_start_time[c, g] < lunch_break_start and groups_start_time[c, g] >= lunch_break_end}
        (x[c, g] * (groups_end_time[c, g] - max(lunch_break_start, groups_start_time[c, g]))))
    +
    (sum{sg in sport_groups: sport_groups_days[sg] == d and
        lunch_break_start <= sport_groups_start_time[sg] and sport_groups_start_time[sg] < lunch_break_end}
        (sport_group_selection[sg] * (min(lunch_break_end, sport_groups_end_time[sg]) - sport_groups_start_time[sg])))
    +
    (sum{sg in sport_groups: sport_groups_days[sg] == d and
        lunch_break_start < sport_groups_end_time[sg] and sport_groups_end_time[sg] <= lunch_break_end
        and sport_groups_start_time[sg] < lunch_break_start and sport_groups_start_time[sg] >= lunch_break_end}
        (sport_group_selection[sg] * (sport_groups_end_time[sg] - max(lunch_break_start, sport_groups_start_time[sg]))))
    <= (lunch_break_end - lunch_break_start) - min_lunch_break_time;

# 4.2 constraints
s.t. min_preference_satisfaction {c in classes, g in groups}:
    groups_preference[c, g] >= x[c, g] * min_preference;

s.t. forbidden_days_satisfaction {c in classes, g in groups}:
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

data;

set classes := "Algebra" "Analiza" "Fizyka" "Chemia min." "Chemia org.";
param max_hours_per_day := 4;
param groups_days :=
    ["Algebra", 1] 1
    ["Algebra", 2] 2
    ["Algebra", 3] 3
    ["Algebra", 4] 3
    ["Analiza", 1] 1
    ["Analiza", 2] 2
    ["Analiza", 3] 3
    ["Analiza", 4] 4
    ["Fizyka", 1] 2
    ["Fizyka", 2] 2
    ["Fizyka", 3] 4
    ["Fizyka", 4] 4
    ["Chemia min.", 1] 1
    ["Chemia min.", 2] 1
    ["Chemia min.", 3] 4
    ["Chemia min.", 4] 5
    ["Chemia org.", 1] 1
    ["Chemia org.", 2] 1
    ["Chemia org.", 3] 5
    ["Chemia org.", 4] 5
    ;
param groups_start_time :=
    ["Algebra", 1] 13
    ["Algebra", 2] 10
    ["Algebra", 3] 10
    ["Algebra", 4] 11
    ["Analiza", 1] 13
    ["Analiza", 2] 10
    ["Analiza", 3] 11
    ["Analiza", 4] 8
    ["Fizyka", 1] 8
    ["Fizyka", 2] 10
    ["Fizyka", 3] 15
    ["Fizyka", 4] 17
    ["Chemia min.", 1] 8
    ["Chemia min.", 2] 8
    ["Chemia min.", 3] 13
    ["Chemia min.", 4] 13
    ["Chemia org.", 1] 9
    ["Chemia org.", 2] 10.5
    ["Chemia org.", 3] 11
    ["Chemia org.", 4] 13
    ;
param groups_end_time :=
    ["Algebra", 1] 15
    ["Algebra", 2] 12
    ["Algebra", 3] 12
    ["Algebra", 4] 13
    ["Analiza", 1] 15
    ["Analiza", 2] 12
    ["Analiza", 3] 13
    ["Analiza", 4] 10
    ["Fizyka", 1] 11
    ["Fizyka", 2] 13
    ["Fizyka", 3] 18
    ["Fizyka", 4] 20
    ["Chemia min.", 1] 10
    ["Chemia min.", 2] 10
    ["Chemia min.", 3] 15
    ["Chemia min.", 4] 15
    ["Chemia org.", 1] 10.5
    ["Chemia org.", 2] 12
    ["Chemia org.", 3] 12.5
    ["Chemia org.", 4] 14.5
    ;
param groups_preference :=
    ["Algebra", 1] 5
    ["Algebra", 2] 4
    ["Algebra", 3] 10
    ["Algebra", 4] 5
    ["Analiza", 1] 4
    ["Analiza", 2] 4
    ["Analiza", 3] 5
    ["Analiza", 4] 6
    ["Fizyka", 1] 3
    ["Fizyka", 2] 5
    ["Fizyka", 3] 7
    ["Fizyka", 4] 8
    ["Chemia min.", 1] 10
    ["Chemia min.", 2] 10
    ["Chemia min.", 3] 7
    ["Chemia min.", 4] 5
    ["Chemia org.", 1] 0
    ["Chemia org.", 2] 5
    ["Chemia org.", 3] 3
    ["Chemia org.", 4] 4
    ;
param sport_groups_days :=
    1 1
    2 3
    3 3
    ;
param sport_groups_start_time :=
    1 13
    2 11
    3 13
    ;
param sport_groups_end_time :=
    1 15
    2 13
    3 15
    ;
param lunch_break_start := 12;
param lunch_break_end := 14;
param min_lunch_break_time := 1;

# 4.2 parameters
param min_preference := 5;
set allowed_days_of_week := 1 2 4;

end;
