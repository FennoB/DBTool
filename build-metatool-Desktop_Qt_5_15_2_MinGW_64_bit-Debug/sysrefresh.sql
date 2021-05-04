  DROP TABLE IF EXISTS _Variables;
  CREATE TABLE _Variables(Name TEXT PRIMARY KEY, RealValue REAL, IntegerValue INTEGER, BlobValue BLOB, TextValue TEXT);
  INSERT INTO _Variables (Name, IntegerValue) VALUES ('wgk0', 0);
  INSERT INTO _Variables (Name, IntegerValue) VALUES ('wgk1', 0);
  INSERT INTO _Variables (Name, IntegerValue) VALUES ('wgk2', 0);
  INSERT INTO _Variables (Name, IntegerValue) VALUES ('wgk3', 0);
  INSERT INTO _Variables (Name, IntegerValue) VALUES ('awg', 0);
  
  UPDATE system SET __001_halle = (
    SELECT name from hierarchy AS hr WHERE EXISTS (
      SELECT 1 FROM hierarchy WHERE id = :hid AND parent_hid = hr.id) LIMIT 1
  )
  WHERE entry_hid = :hid;

  UPDATE system SET __002_volumen = (
     SELECT CASE WHEN (SUM(c.__001_volumen) IS NOT NULL) THEN
       SUM(c.__001_volumen) 
       ELSE 0 
     END
	 FROM container c 
	 LEFT JOIN hierarchy hr ON hr.id = c.entry_hid 
	 WHERE hr.parent_hid IN (
	    SELECT h.id FROM subsystem s 
      LEFT JOIN hierarchy h ON s.entry_hid = h.id 
      WHERE h.parent_hid = :hid
	 )
  )
  WHERE entry_hid = :hid;
  
  UPDATE _Variables SET RealValue = (
	SELECT (
		CASE WHEN (SUM(c.__001_volumen) IS NOT NULL) THEN
		SUM(c.__001_volumen) ELSE 0 END 
		/ (SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1)
	)
	AS sm FROM container c
	LEFT JOIN hierarchy hr ON hr.id = c.entry_hid
	WHERE __003_wgk='nwg' and hr.parent_hid IN (
		SELECT h.id FROM subsystem s
		LEFT JOIN hierarchy h ON s.entry_hid = h.id
		WHERE h.parent_hid = :hid)
  ) 
  WHERE Name = 'wgk0';
  
  UPDATE _Variables SET RealValue = (
	SELECT (
		CASE WHEN (SUM(c.__001_volumen) IS NOT NULL) THEN
		SUM(c.__001_volumen) ELSE 0 END 
		/ (SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1)
	)
	AS sm FROM container c
	LEFT JOIN hierarchy hr ON hr.id = c.entry_hid
	WHERE __003_wgk='1' and hr.parent_hid IN (
		SELECT h.id FROM subsystem s
		LEFT JOIN hierarchy h ON s.entry_hid = h.id
		WHERE h.parent_hid = :hid)
  ) 
  WHERE Name = 'wgk1';
  
  UPDATE _Variables SET RealValue = (
	SELECT (
		CASE WHEN (SUM(c.__001_volumen) IS NOT NULL) THEN
		SUM(c.__001_volumen) ELSE 0 END 
		/ (SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1)
	)
	AS sm FROM container c
	LEFT JOIN hierarchy hr ON hr.id = c.entry_hid
	WHERE __003_wgk='2' and hr.parent_hid IN (
		SELECT h.id FROM subsystem s
		LEFT JOIN hierarchy h ON s.entry_hid = h.id
		WHERE h.parent_hid = :hid)
  ) 
  WHERE Name = 'wgk2';
  
  UPDATE _Variables SET RealValue = (
	SELECT (
		CASE WHEN (SUM(c.__001_volumen) IS NOT NULL) THEN
		SUM(c.__001_volumen) ELSE 0 END 
		/ (SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1)
	)
	AS sm FROM container c
	LEFT JOIN hierarchy hr ON hr.id = c.entry_hid
	WHERE __003_wgk='3' and hr.parent_hid IN (
		SELECT h.id FROM subsystem s
		LEFT JOIN hierarchy h ON s.entry_hid = h.id
		WHERE h.parent_hid = :hid)
  )
  WHERE Name = 'wgk3';

  UPDATE _Variables SET RealValue = (
	SELECT (
		CASE WHEN (SUM(c.__001_volumen) IS NOT NULL) THEN
		SUM(c.__001_volumen) ELSE 0 END 
		/ (SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1)
	)
	AS sm FROM container c
	LEFT JOIN hierarchy hr ON hr.id = c.entry_hid
	WHERE __003_wgk='awg' and hr.parent_hid IN (
		SELECT h.id FROM subsystem s
		LEFT JOIN hierarchy h ON s.entry_hid = h.id
		WHERE h.parent_hid = :hid)
  ) 
  WHERE Name = 'awg';
  
  UPDATE system SET __003_wgk = (
  SELECT CASE
    WHEN (SELECT RealValue FROM _Variables WHERE Name = 'awg' LIMIT 1) > 0.99 THEN 'awg'
    WHEN (SELECT RealValue FROM _Variables WHERE Name = 'awg' LIMIT 1) > 0 THEN 'Es ist kompliziert...'
    WHEN (SELECT RealValue FROM _Variables WHERE Name = 'wgk3' LIMIT 1) > 0.03 THEN '3'
    WHEN (SELECT RealValue FROM _Variables WHERE Name = 'wgk3' LIMIT 1) > 0 THEN '2'
    WHEN (SELECT RealValue FROM _Variables WHERE Name = 'wgk2' LIMIT 1) > 0.03 THEN '2'
    WHEN (SELECT RealValue FROM _Variables WHERE Name = 'wgk2' LIMIT 1) > 0 THEN '1'
    WHEN (SELECT RealValue FROM _Variables WHERE Name = 'wgk1' LIMIT 1) > 0.03 THEN '1'
    ELSE 'nwg'
  END);
  
  UPDATE system SET __004_gefstufe = (
  (SELECT CASE
    WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '1' AND
		(SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1) <= 100 THEN 'Stufe A'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '1' AND
		(SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1) <= 1000 THEN 'Stufe B'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '1' THEN 'Stufe C'
	
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '2' AND
		(SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1) <= 1 THEN 'Stufe A'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '2' AND
		(SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1) <= 10 THEN 'Stufe B'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '2' AND
		(SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1) <= 100 THEN 'Stufe C'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '2' THEN 'Stufe D'
	
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '3' AND
		(SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1) <= 0.22 THEN 'Stufe A'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '3' AND
		(SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1) <= 1 THEN 'Stufe B'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '3' AND
		(SELECT __002_volumen FROM system WHERE entry_hid = :hid LIMIT 1) <= 10 THEN 'Stufe C'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = '3' THEN 'Stufe D'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = 'Es ist kompliziert...' THEN 'Es ist kompliziert...'
	WHEN (SELECT __003_wgk FROM system WHERE entry_hid = :hid LIMIT 1) = 'awg' THEN 'Keine gefstuf'
    ELSE 'Keine AwSV Anlage'
  END));
  
  INSERT INTO _Variables (Name, IntegerValue) VALUES ('sys_unterird', (
    SELECT COUNT(*) FROM container c
    LEFT JOIN hierarchy h ON h.id = c.entry_hid
    WHERE c.__004_unterirdisch="Ja" AND h.parent_hid IN 
    (
      SELECT entry_hid FROM subsystem s
      LEFT JOIN hierarchy hr ON hr.id = s.entry_hid
      WHERE hr.parent_hid = :hid
    )
  ));
  
  INSERT INTO _Variables (Name, TextValue) VALUES ('next_pf_5', 
  (
    SELECT STRFTIME
    (
      '%Y-%m-%dT%H:%M:%S',
      DATETIME
      (
        (SELECT DATETIME(__007_letzte_prf) FROM system WHERE entry_hid = :hid LIMIT 1), 
        '+5 years'
      )
    )
  ));

  INSERT INTO _Variables (Name, TextValue) VALUES ('next_pf_30m', 
  (
    SELECT STRFTIME
    (
      '%Y-%m-%dT%H:%M:%S',
      DATETIME
      (
        (SELECT DATETIME(__007_letzte_prf) FROM system WHERE entry_hid = :hid LIMIT 1), 
        '+30 months'
      )
    )
  ));
  
  
  UPDATE system SET __008_naechste_prf = (
    SELECT CASE
      WHEN (SELECT IntegerValue FROM _Variables WHERE Name = 'sys_unterird' LIMIT 1) > 0
      THEN (SELECT TextValue FROM _Variables WHERE Name = 'next_pf_5' LIMIT 1)
      WHEN (SELECT IntegerValue FROM _Variables WHERE Name = 'sys_unterird' LIMIT 1) = 0
        AND (SELECT __004_gefstufe FROM system WHERE entry_hid = :hid LIMIT 1) IN ("Stufe C", "Stufe D")
      THEN (SELECT TextValue FROM _Variables WHERE Name = 'next_pf_30m' LIMIT 1)
      ELSE 'Keine Pflicht'
    END
  );
  
  INSERT INTO _Variables (Name, IntegerValue) VALUES ('days_to_next_pf', 
  (
    SELECT julianday(
    (
      SELECT __008_naechste_prf FROM system WHERE entry_hid = :hid LIMIT 1
    )) - julianday('now')
  ));
  
  UPDATE system SET __009_color = ( 
    SELECT CASE
      WHEN (SELECT __008_naechste_prf FROM system WHERE entry_hid = :hid LIMIT 1) = "Keine Pflicht" THEN '#00F'
      WHEN (SELECT IntegerValue FROM _Variables WHERE Name = 'days_to_next_pf' LIMIT 1) > 90 THEN '#0F0'
      WHEN (SELECT IntegerValue FROM _Variables WHERE Name = 'days_to_next_pf' LIMIT 1) > 30 THEN '#FF0'
      WHEN (SELECT IntegerValue FROM _Variables WHERE Name = 'days_to_next_pf' LIMIT 1) > 14 THEN '#F80'
      ELSE '#F00'
    END
  );