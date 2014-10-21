


DokXmlWriter *dok_xml_writer_new ( FILE *file );
void dok_xml_writer_open_node ( DokXmlWriter *writer,
                                const gchar *name );
void dok_xml_writer_set_attribute ( DokXmlWriter *writer,
                                    const gchar *name,
                                    const gchar *value );
void dok_xml_writer_add_text ( DokXmlWriter *writer,
                               const gchar *text );
void dok_xml_writer_close_node ( DokXmlWriter *writer );
