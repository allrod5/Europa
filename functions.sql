USE matriculasrff;

delimiter @!!
drop function if exists genCampusId @!!
create function genCampusId() returns int

begin 

declare newId int;

if (not exists (select id from campus)) then
    return 1;
end if;

select max(id) into newId from campus;

set newId = newId + 1;

return newId;

end @!!
delimiter ;

delimiter @!!
drop function if exists genTurmaId @!!
create function genTurmaId() returns int

begin 

declare newId int;

if (not exists (select id from campus)) then
    return 1;
end if;

select max(id) into newId from campus;

set newId = newId + 1;

return newId;

end @!!
delimiter ;

delimiter @!!
drop function if exists genEnderecoId @!!
create function genEnderecoId() returns int

begin 

declare newId int;

if (not exists (select id from endereco)) then
    return 1;
end if;

select max(id) into newId from endereco;

set newId = newId + 1;

return newId;

end @!!
delimiter ;

delimiter @!!
drop function if exists genProfessorSiape @!!
create function genProfessorSiape() returns int

begin 

declare newSiape int;

if (not exists (select siape from professor)) then
    return 1;
end if;

select max(siape) into newSiape from professor;

set newSiape = newSiape + 1;

return newSiape;

end @!!
delimiter ;


delimiter @!!
drop function if exists curQuadrimestreId @!!
create function curQuadrimestreId() returns int

begin 

declare maxId int;

if (not exists (select id from quadrimestre)) then
    return 0;
end if;

select max(id) into maxId from quadrimestre;

return maxId;

end @!!
delimiter ;



