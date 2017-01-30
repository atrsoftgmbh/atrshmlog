--
-- $Id:$
--
-- create the role atrshmlog with login
-- needs the authentication set to password and the rol to ...
-- see my conf
CREATE ROLE atrshmlog WITH PASSWORD = 'atrshmlog4711'
    AND LOGIN = true;
